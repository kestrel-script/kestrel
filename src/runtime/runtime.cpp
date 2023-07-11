#include "runtime/runtime.hpp"
#include "runtime/interpreter.hpp"

namespace kestrel {

struct Runtime::Detail {
    std::unordered_map<std::string, Module> globals;
    Interpreter interpreter;
};

Runtime::Runtime() : detail(std::make_unique<Detail>()) {};
Runtime::~Runtime() = default;

Module& Runtime::defineModule(const std::string& name) {
    detail->globals.emplace(std::make_pair(name, Module()));
    return detail->globals[name];
};

Value Runtime::run(Module& module, Function& function) {
    detail->interpreter.run(module, function);
    return Value::nil(); // TODO
}

}