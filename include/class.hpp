#pragma once

#include <memory>
#include <functional>
#include <unordered_map>
#include <map>

namespace kestrel {

class Value;

struct MethodParameter {
    std::vector<Value>& args; // positional arguments;
    // std::map<String, Value>& kvargs;

    // a.foo(a = 50, b = 300);
    // a.foo {a = 50, b = 300};
};

using Method = std::function<Value(Value&, MethodParameter&)>;
// using Constructor = std::function<Value(MethodParameter&)>;
using Constructor = Method;

class Class {
public:

    Value dispatch(Value& self, std::string& name, MethodParameter& );
    void registerMethod(const std::string& name, Method&& method);

    Value construct(Value&, MethodParameter&);

    void constructor(Constructor m) {
        constructor_ = m;
    }

protected:
    std::map<std::string, Method> methods;
    Constructor constructor_; // TODO 
};

}