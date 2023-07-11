#pragma once

#include <map>
#include <memory>

#include "module.hpp"
#include "statement.hpp"

#include "instruction_array.hpp"
#include "opcodes.hpp"

namespace kestrel {

using namespace kestrel;

class Compiler {
public:

  Compiler();
  ~Compiler();

  Module compile(std::vector<std::shared_ptr<Statement>> &statemetns);

  InstructionArray &instructions();

  std::shared_ptr<Module> module();

  void setModule(std::shared_ptr<Module> module);

  // TODO move to codegen?
  void emitCode(Opcode code);

  int emitIndex(int16_t index);

  int putConstant(Value& v);

  int lookup(const std::string &name);

  // int defineVaraible(const std::string& name);

  void makeGlobal(const std::string&, Value& value);

  int addLocal(const std::string&);

  int maxSlots() const;

  void enterBlock();

  void exitBlock();


  int nameIndex(const std::string& name);

private:
  struct Detail;
  std::unique_ptr<Detail> detail;
};

} // namespace ir
