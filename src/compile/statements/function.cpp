
#include "function.hpp"

#include <memory>

#include "compile/statements.hpp"
#include "compiler.hpp"

namespace kestrel {

void FunctionStatement::prepare(Compiler &compiler) {

}

void FunctionStatement::evaluate(Compiler& compiler) {
    Log(level, tag) << "FunctionStatement" ;
    
    Compiler subCompiler;
    subCompiler.setModule(compiler.module());

    for (int i = 0; i < params_.size(); i++) {
        subCompiler.addLocal(params_[i].lexeme);
    }

    Module m = subCompiler.compile(body_);

    // TODO add nil?
    m.instructions.appendByte((uint8_t)Opcode::Return);
    // subCompiler.emitCode(Opcode::Return);

    Log(level, tag) << "start function" ;
    Log(level, tag) << "name:" << name_.lexeme ;
    Log(level, tag) << "arty:" << params_.size() ;
    Log(level, tag) << "body.size:" << body_.size() ;
    Log(level, tag) << "instructions.size:" << m.instructions.size();

    std::shared_ptr<Function> function;
    function = std::make_shared<Function>(m.instructions);
    function->setArity(params_.size()); // TODO store names for kvargs?
    function->setName(name_.lexeme);
    function->setMaxSlots(subCompiler.maxSlots());
    
    Value value(function);
    // TODO global?
    Log(level, tag) << "before make global" ;
    compiler.makeGlobal(name_.lexeme, value);
}

void FunctionStatement::print() {
    Log(level, tag) << "Function " << name_.toString() ;
    Log(level, tag) << "Params : (";
    for (auto t : params_) {
        Log(level, tag) << t.toString() << ",";
    }
    Log(level, tag) << ")";
    Log(level, tag) ;
    Log(level, tag) << "statements:" ;
    for (auto stmt : body_) {
        stmt->print();
    }
}


}