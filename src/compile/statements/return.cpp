#include "compile/statements.hpp"
#include "compiler.hpp"

namespace kestrel {
void ReturnStatement::evaluate(Compiler& compiler) {
  // std::cout << "ReturnStatement:" << std::endl;
  if (value) {
    value->eval(compiler);
  } else {
    compiler.emitCode(Opcode::LoadNil);
  }
  compiler.emitCode(Opcode::Return);
}

}