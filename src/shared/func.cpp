#include "function.hpp"

namespace kestrel {

class Function::Detail {
public:
    InstructionArray instructions;
    bool isInitializer;
    int arity = 0;
    int localSize = 0;
    FunctionType type = Native;
    ForeignFunction foreignFunction_;
    std::string name;
};

Function::Function() : detail(std::make_unique<Detail>()) {}

Function::Function(ForeignFunction f) : detail(std::make_unique<Detail>()) {
    detail->type = Foreign;
    detail->foreignFunction_ = f;
}

Function::Function(InstructionArray& instructions) : detail(std::make_unique<Detail>()) {
    detail->type = Native;
    detail->instructions = instructions;
}

Function::~Function() = default;

Function::Function(const Function& other) : detail(std::make_unique<Detail>(*other.detail)) {}

Function& Function::operator=(const Function& other) {
    if (&other != this) {
        detail = std::make_unique<Detail>(*other.detail);
    }
    return *this;
}

Function::Function(Function&& other) noexcept = default;

Function& Function::operator=(Function&& other) noexcept = default;

FunctionType Function::type() const {
    return detail->type;
}

ForeignFunction& Function::foreignFunction() {
    return detail->foreignFunction_;
}

std::string Function::toString() const {
    if (type() == Native) {
        return "<native function>";
    } else {
        return "<foreign function>";
    }
}

void Function::setArity(size_t arity) {
    detail->arity = static_cast<int>(arity);
}

int Function::arity() const {
    return detail->arity;
}

int Function::maxSlots() const {
    return detail->localSize;
}

void Function::setMaxSlots(int size) {
    detail->localSize = size;
}

void Function::setName(const std::string& name) {
    detail->name = name;
}

std::string Function::name() const {
    return detail->name;
}

InstructionArray& Function::instructions() {
    return detail->instructions;
}

}  // namespace kestrel