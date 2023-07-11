#include "compiler.hpp"

#include <unordered_map>

#include "log.hpp"
#include "module.hpp"
#include "statement.hpp"

#include "instruction_array.hpp"
#include "opcodes.hpp"
#include "statements.hpp"
#include "symbol_table.hpp"

namespace kestrel {

struct Compiler::Detail {

  std::shared_ptr<Module> module_ = std::make_shared<Module>();
  InstructionArray instructions;
  // std::vector<Value> constants;
  std::vector<std::string> names; // global name table;
  // std::unordered_map<std::string, Value> globals;
  SymbolTable table;

};

Compiler::Compiler() : detail(std::make_unique<Detail>()) {}

Compiler::~Compiler() = default;

InstructionArray& Compiler::instructions() {
  return detail->instructions;
}

Module Compiler::compile(std::vector<std::shared_ptr<Statement>> &statemetns) {

  for (auto &statemnt : statemetns) {
    Log(level, tag) << statemnt;
    std::cout <<  statemnt << std::endl;
    statemnt->print();
    statemnt->prepare(*this);
    statemnt->evaluate(*this);
    // TODO pop the last result?
  }
  Module m; // TODO
  m.instructions = detail->instructions;
  m.constants = detail->module_->constants;
  m.names = detail->module_->names;
  m.globals_ = detail->module_->globals_; //TODO
  m.initializer_ = Function(detail->instructions);
  return m;
  // TODO
}

std::shared_ptr<Module> Compiler::module() {
  return detail->module_;
}

void Compiler::setModule(std::shared_ptr<Module> module_) {
  detail->module_ = module_;
}

void Compiler::emitCode(Opcode code) {
  detail->instructions.appendByte(static_cast<uint8_t>(code));
}

int Compiler::emitIndex(int16_t index) {
  detail->instructions.appendShort(index);
  return (int) detail->instructions.size() - 2;
}

int Compiler::putConstant(Value& v) {
  return detail->module_->putConstant(v);
}

int Compiler::lookup(const std::string &name) {
  return detail->table.findSymbol(name);
}

void Compiler::makeGlobal(const std::string& name, Value& value) {
  detail->module_->setGlobal(name, value);
}

int Compiler::addLocal(const std::string& value) {
  return detail->table.addSymbol(value);
}

int Compiler::maxSlots() const {
  return detail->table.maxSlots();
}

void Compiler::enterBlock() {
  detail->table.enterScope(); // TODO
}

void Compiler::exitBlock() {
  detail->table.exitScope();
}

int Compiler::nameIndex(const std::string& name) {
  return detail->module_->nameIndex(name);
}

// void Compiler::emitLoadVariable() {

// }

} // namespace ir
