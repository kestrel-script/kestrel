#include "compile/statements.hpp"
#include "compiler.hpp"

namespace kestrel {
void IfStatement::evaluate(Compiler& compiler) {
    // condition first
    condition->eval(compiler);
    compiler.emitCode(Opcode::BranchFalse);
    int index = compiler.emitIndex(0);

    int current = (int) compiler.instructions().size();

    thenBranch->evaluate(compiler);
    int offset = (int) compiler.instructions().size() - current;
    compiler.instructions().writeShort(offset, index); // patch offset;

    Log(level, tag) << "then is:";
    thenBranch->print();

    if (elseBranch) {
        elseBranch->evaluate(compiler);
    }
    
}

}
