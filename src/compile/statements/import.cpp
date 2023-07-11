#include "compile/statements.hpp"
#include "compiler.hpp"

namespace kestrel {
void Import::evaluate(Compiler& compiler) {
    std::cout << "Import:" << this->name.lexeme << std::endl;

    int index = compiler.nameIndex(name.lexeme);
    compiler.emitCode(Opcode::Import);
    compiler.emitIndex(index);
}

}
