#include "class.hpp"

#include <vector>
#include <string>

#include <iostream>
#include "value.hpp"

namespace kestrel {

Value Class::dispatch(Value& self, std::string& name, MethodParameter& params) {
    int count = methods.count(name);
    if (count > 0) {
        Method& method = this->methods[name];
        return method(self, params);
    } else {
        // TODO error;
        return Value::nil();
    }
}

void Class::registerMethod(const std::string& name, Method&& method) {
    // TODO warp in a function;
    methods[name] = method;
}

Value Class::construct(Value& val, MethodParameter& params) {
    return constructor_(val, params);
}

}