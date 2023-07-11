#include "statements.hpp"
#include "compiler.hpp"
#include "function.hpp"

namespace kestrel {

void ExpressionStatement::evaluate(Compiler &compiler) {
  Log(level, tag) << "ExpressionStatement";
  expression->eval(compiler);
}

void BlockStatement::evaluate(Compiler &compiler) {
  Log(level, tag) << "BlockStatement";
  compiler.enterBlock();
  for (auto &statement : statements) {
    statement->evaluate(compiler);
  }
  compiler.exitBlock();
}

void VariableStatement::evaluate(Compiler &compiler) {
  Log(level, tag) << "VariableStatement " << name.lexeme;
  if (initializer) {
    initializer->eval(compiler);
  } else {
    compiler.emitCode(Opcode::LoadNil);
  }

  int index = compiler.lookup(name.lexeme); // local? global?
  if (index != -1) {
    // TODO already exist;
  }
  // TODO make global?
  index = compiler.addLocal(name.lexeme);
  // store local or global;
  compiler.emitCode(Opcode::Store);
  compiler.emitIndex(index);
}

} // namespace kestrel