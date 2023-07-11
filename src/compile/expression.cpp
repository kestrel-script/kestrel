#include "expression.hpp"
#include "compiler.hpp"
#include "opcodes.hpp"
#include "log.hpp"

namespace kestrel {

static LogLevel level = LogLevel::Debug;
static std::string tag = "expr";

void Assign::eval(Compiler &compiler) {
  Log(level, tag) << "Assign : " << name.lexeme;

  value->eval(compiler);
}

void Binary::eval(Compiler &compiler) {
  Log(level, tag) << "Binary:";

  static std::map<std::string, Opcode> opcodeMap = {
      {"+", Opcode::Add},
      {"-", Opcode::Subtract},
      {"*", Opcode::Multiply},
      {"/", Opcode::Divide},
      /** Logical operators */
      {"<", Opcode::LessThan},
      {">", Opcode::GreaterThan},

      {"==", Opcode::Equals},
  };

  left->eval(compiler);
  right->eval(compiler);
  Log(level, tag) << "operator : " << op.lexeme;
  Opcode opcode = opcodeMap[op.lexeme];
  compiler.emitCode(opcode);
}

void Call::eval(Compiler &compiler) {
  callee->eval(compiler); 
  for (int i = 0; i < arguments.size(); i++) {
    arguments[i]->eval(compiler);
  }
  
  compiler.emitCode(Opcode::Call);
  compiler.emitIndex(arguments.size());
}

void Dispatch::eval(Compiler &compiler) {
  std::cout << "Calling method:" << name.lexeme << std::endl;
  object->eval(compiler); 
  for (int i = 0; i < arguments.size(); i++) {
    arguments[i]->eval(compiler);
  }
  
  int index = compiler.nameIndex(name.lexeme);
  // compiler.emitCode(Opcode::LoadConstant);
  // compiler.emitIndex(index);
  // TODO encode into bytecode? Dispatch, name, argCount;

  compiler.emitCode(Opcode::Dispatch);
  compiler.emitIndex(index);
  compiler.emitIndex(arguments.size());
}

void Get::eval(Compiler &compiler) {
  std::cout << "Get:" << name.lexeme << std::endl;
  object->eval(compiler);
  int index = compiler.nameIndex(name.lexeme);

  compiler.emitCode(Opcode::LoadName);
  compiler.emitIndex(index);
  compiler.emitCode(Opcode::GetItem);

  // compiler.emitCode(Opcode::Load)
  Log(level, tag) << "Get";

}

void Set::eval(Compiler &compiler) {
  Log(level, tag) << "Set";
}

void Variable::eval(Compiler &compiler) {
  Log(level, tag) << "Variable: " << name.lexeme;
  int index = compiler.lookup(name.lexeme);
  // std::cout << name.lexeme << " at index " << index << std::endl;
  if (index >= 0) {
    // local variable
    compiler.emitCode(Opcode::LoadLocal);
    compiler.emitIndex(index);
  } else {
    // global variable;
    // Value val(name.lexeme);
    // int index = compiler.putConstant(val);
    int index = compiler.nameIndex(name.lexeme);
    // std::cout << "constant:" << index << std::endl;
    compiler.emitCode(Opcode::LoadGlobal); // TODO LoadName?
    compiler.emitIndex(index);
  }
}

void Super::eval(Compiler &compiler) {
  Log(level, tag) << "Super";
}

void This::eval(Compiler &compiler) {
  Log(level, tag) << "This";
  // return 0;
}

void Unary::eval(Compiler &compiler) {
  Log(level, tag) << "Unary";
  // return 0;
}

void Logical::eval(Compiler &compiler) {
  Log(level, tag) << "Logical";
  // return 0;
}

void Grouping::eval(Compiler &compiler) {
  Log(level, tag) << "Grouping";
  // return 0;
}

void LiteralExpression::eval(Compiler &compiler) {
  Log(level, tag) << "literal exprssion :" << value->toString() << " type:" << (int)value->type();

  switch (value->type()) {
      // TODO
      case ValueType::Integer:
      case ValueType::Double: {
        compiler.emitCode(Opcode::LoadInteger);
        compiler.emitIndex(value->intValue());
        break;
      }

      case ValueType::Boolean: //TODO
      case ValueType::String: 
      case ValueType::Nil : {
        int index = compiler.putConstant(*value);
        compiler.emitCode(Opcode::LoadConstant);
        compiler.emitIndex(index);
        break;
      }
      case ValueType::Function: {
          // TODO
          break;
      }
      default: {
        // std::cout << "type:" << (int)value->type() << std::endl;
        throw -1;
      }
  }

}

} // namespace kestrel
