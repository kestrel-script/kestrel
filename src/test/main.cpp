#include <iostream>

#include <fstream>

#include "function.hpp"

#include "core/object.hpp"

#include "compile/parser.hpp"
#include "compile/scanner.hpp"
#include "compiler.hpp"

#include "runtime/runtime.hpp"

using namespace kestrel;

size_t intWidth(size_t num) { return std::to_string(num).length(); }

String readFileIntoString(const std::string &path) {
  std::ifstream input_file(path);
  if (!input_file.is_open()) {
    std::cerr << "Could not open the file - '" << path << "'";
    exit(EXIT_FAILURE);
  }
  return String((std::istreambuf_iterator<char>(input_file)),
                std::istreambuf_iterator<char>());
}

bool log_enabled = true;

int main(int argc, char **argv) {

  static Writer writer = [](std::string& s) {
    if (log_enabled) {
      std::cout << yellow << s << reset;
    }
  };

  setWriter(writer);

  std::string path = std::string(argv[1]);
  std::string source = readFileIntoString(path);
  Scanner scanner(source);
  std::vector<Token> tokens = scanner.scanTokens();
  for (Token &token : tokens) {
    Log(level, tag) << token.toString() ;
  }


  Parser parser(tokens);
  auto statements = parser.parse();
  for (auto statement : statements) {
    statement->print();
  }

  // return 0;

  Compiler compiler;
  Module module_ = compiler.compile(statements);

  for (int i = 0; i < module_.instructions.size(); i++) {
    // std::cout << ":" << (int)module_.instructions.readByte(i) << std::endl;
  }

  // return 0;

  ForeignFunction same = [](std::vector<Value> &args) {
    return Value(args[0] == args[1]);
  };

  ForeignFunction enableLog = [](std::vector<Value> &args) {
    log_enabled = (int)args[0];
    return Value();
  };

  ForeignFunction assertF = [](std::vector<Value> &args) {
    // // std::cout << "args:" << args.size() << " : " << args[0].asBoolean() << std::endl;
    assert(args.size() == 1);
    if(args[0].boolValue() == true) {
      // std::cout << "PASS!" << std::endl;
    } else {
      // std::cout << "FAIL!" << std::endl;
    }
    
    return Value(true);
  };

  ForeignFunction print = [](std::vector<Value> &args) {
    std::cout << "printing : ";
    std::cout << args[0].toString() << std::endl;
    return Value::nil();
  };

  
  Value p(print);
  p.functionValue()->setName("print");
  
  module_.setGlobal("print", p);


  // module_.setGlobal("a", Value(same));
  module_.setGlobal("enableLog", Value(enableLog));
  module_.setGlobal("assert", assertF);

  // Value& v = module_.getGlobal("print");
  // // std::cout << "v:" << v << std::endl;

  Log(level, tag) << "running..........";
  // Interpreter vm;

  ForeignFunction time = [](std::vector<Value> &args) {
    std::cout << "time... ";
    // std::cout << args[0].toString() << std::endl;
    return Value(100);
  };

  Runtime runtime;

  Class* calculator = new Class();
  calculator->constructor([&](Value& cls, MethodParameter& p) {
    core::Object* object = new core::Object(); // TODO construct from Class? default contructor;
    object->setClass(calculator);
    return Value(object);
  });
  calculator->registerMethod("desc", [](Value& self, MethodParameter& p) {
    return "I'm a calculator, I can perform add,sub,multiply and divide operation.";
  });
  calculator->registerMethod("add", [](Value& self, MethodParameter& p) {
    assert(p.args.size() == 2);
    assert(p.args[0].isNumber() && p.args[1].isNumber());
    return p.args[0].doubleValue() + p.args[1].doubleValue();
  });
  calculator->registerMethod("sub", [](Value& self, MethodParameter& p) {
    return p.args[0].doubleValue() - p.args[1].doubleValue();
  });
  calculator->registerMethod("mul", [](Value& self, MethodParameter& p) {
    return p.args[0].doubleValue() * p.args[1].doubleValue();
  });
  calculator->registerMethod("div", [](Value& self, MethodParameter& p) {
    return p.args[0].doubleValue() / p.args[1].doubleValue();
  });
  
  module_.setGlobal("Calculator", Value(calculator));
  runtime.run(module_, module_.initializer_);
  
};
