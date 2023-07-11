#pragma once

#include <functional>
#include <memory>
#include <string>
#include <ostream>
#include <vector>

#include "class.hpp"
#include "core/object.hpp"

namespace kestrel {

using Object = core::Object;

class Function;

enum class ValueType : unsigned char {
    Nil = 0,
    Boolean,
    Integer,
    Double,
    String,
    Function,
    Closure,
    Object,
    Module,
    Class,
    Unknown,
};

class Value {
public:
    Value();
    Value(int value);
    Value(bool value);
    Value(const Value& other);
    Value(double value);
    Value(std::string value);
    Value(const char*);
    Value(std::function<Value(std::vector<Value>&)> value); // ForeignFunction;
    Value(const std::shared_ptr<Function>& f);
    Value(Class* cls);
    Value(Object*);
    ~Value();

    bool isNumber() const;
    ValueType type() const;

    operator bool() const;
    operator int() const;
    operator double() const;
    operator std::string&() const;
    operator std::function<Value(std::vector<Value>&)>() const;

    bool boolValue() const;
    int intValue() const;
    double doubleValue() const;
    std::string& stringValue() const;
    std::shared_ptr<Function>& functionValue() const;

    void set(bool value);
    void set(int value);
    void set(double value);
    void set(std::string& str); // TODO &&
    void set(std::function<Value(std::vector<Value>&)> function);
    void set(const std::shared_ptr<Function>& f);

    Value& operator=(const bool& other);
    Value& operator=(const int& other);
    Value& operator=(const double& other);
    Value& operator=(const std::string& rhs);
    Value& operator=(const std::function<Value(std::vector<Value>&)>& rhs);
    
    Value& operator=(const Value& rhs);
    Value& operator=(Value&& other) noexcept;

    bool operator==(const Value& rhs) const;
    bool operator!=(const Value& other) const;

    Value& operator[](const std::string& name);

    Class* metaClass() const;

    std::string toString() const;

    friend std::ostream& operator<<(std::ostream& os, const Value& value);

    static Value& nil();
private:
    struct Detail;
    std::shared_ptr<Detail> detail;
};

}