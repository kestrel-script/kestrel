#include "value.hpp"
#include "function.hpp"

#include <sstream>

#include "core/classes.hpp"
#include "core/object.hpp"
#include "core/string.hpp"
#include <iostream>

namespace kestrel {

class DataHolder {
public:
  union {
    bool booleanValue;
    int intValue;
    double doubleValue;
    Class* classPointer;
  };
  // TODO use a pointer?

  core::Object *object;
  std::string stringValue;
  std::shared_ptr<Function> functionPointer;
};

struct Value::Detail {
public:
  Detail() : mutable_(true), type(ValueType::Nil) {
    // std::cout << "holder.size:" << sizeof(Detail) << std::endl;
  }

  bool mutable_;
  ValueType type = ValueType::Nil;
  DataHolder holder_;
};

Value::Value() : detail(std::make_shared<Detail>()) {}

Value::Value(int value) : detail(std::make_shared<Detail>()) { set(value); }

Value::Value(bool value) : detail(std::make_shared<Detail>()) { set(value); }

Value::Value(const Value &other) : detail(std::make_shared<Detail>()) {
  // std::cout << "other 11:" << other.detail.get() << std::endl;
  if (other.detail) {
    detail->mutable_ = other.detail->mutable_;
    detail->type = other.detail->type;
    detail->holder_ = other.detail->holder_;
  }
}

Value::Value(double value) : detail(std::make_shared<Detail>()) { set(value); }

Value::Value(std::string value) : detail(std::make_shared<Detail>()) {
  set(value);
}

Value::Value(const char* value) : detail(std::make_shared<Detail>()) {
  std::string str = value;
  set(str); // TODO rvalue;
}

Value::Value(std::function<Value(std::vector<Value> &)> value)
    : detail(std::make_shared<Detail>()) {
  set(value);
}

Value::Value(const std::shared_ptr<Function> &f)
    : detail(std::make_shared<Detail>()) {
  set(f);
}

Value::Value(Class* cls) : detail(std::make_shared<Detail>()) {
  detail->holder_.classPointer = cls;
  detail->type = ValueType::Class;
}

Value::Value(Object* self) : detail(std::make_shared<Detail>()) {
  // set(cls);
  detail->holder_.object = self;
  detail->type = ValueType::Object;
}

Value::~Value() = default;

bool Value::isNumber() const {
  return detail->type == ValueType::Double ||
         detail->type == ValueType::Integer;
}

ValueType Value::type() const { return detail->type; }

Value::operator bool() const { return boolValue(); }

Value::operator int() const { return intValue(); }

Value::operator double() const { return doubleValue(); }

Value::operator std::string &() const { return stringValue(); }

Value::operator std::function<Value(std::vector<Value> &)>() const {
  return functionValue()->foreignFunction();
}

bool Value::boolValue() const {
  if (detail->type == ValueType::Boolean) {
    return detail->holder_.booleanValue;
  } else if (detail->type == ValueType::Integer) {
    return detail->holder_.intValue != 0;
  } else if (detail->type == ValueType::Double) {
    return detail->holder_.doubleValue != 0.0;
  } else {
    return false;
  }
}

int Value::intValue() const {
  if (detail->type == ValueType::Boolean) {
    return detail->holder_.booleanValue ? 1 : 0;
  } else if (detail->type == ValueType::Integer) {
    return detail->holder_.intValue;
  } else if (detail->type == ValueType::Double) {
    return static_cast<int>(detail->holder_.doubleValue);
  } else {
    return 0;
  }
}

double Value::doubleValue() const {
  if (detail->type == ValueType::Boolean) {
    return detail->holder_.booleanValue ? 1.0 : 0.0;
  } else if (detail->type == ValueType::Integer) {
    return static_cast<double>(detail->holder_.intValue);
  } else if (detail->type == ValueType::Double) {
    return detail->holder_.doubleValue;
  } else {
    return 0.0;
  }
}

std::string &Value::stringValue() const {
  if (detail->type == ValueType::String) {
    return detail->holder_.stringValue;
  } else {
    static std::string empty = "";
    return empty;
  }
}

std::shared_ptr<Function> &Value::functionValue() const {
  if (detail->type == ValueType::Function) {
    return (detail->holder_.functionPointer);
  } else {
    static std::shared_ptr<Function> s; // TODO
    return s;
  }
}

void Value::set(bool value) {
  detail->type = ValueType::Boolean;
  detail->holder_.booleanValue = value;
}

void Value::set(int value) {
  detail->type = ValueType::Integer;
  detail->holder_.intValue = value;
}

void Value::set(double value) {
  detail->type = ValueType::Double;
  detail->holder_.doubleValue = value;
}

void Value::set(std::string &str) {
  detail->type = ValueType::String;
  // TODO
  detail->holder_.stringValue = str;
}

void Value::set(std::function<Value(std::vector<Value> &)> function) {
  detail->type = ValueType::Function;
  // TODO
  detail->holder_.functionPointer = std::make_shared<Function>(function);
}

void Value::set(const std::shared_ptr<Function> &f) {
  detail->type = ValueType::Function;
  detail->holder_.functionPointer = f;
}

Class *Value::metaClass() const {
  // TODO use an array?
  switch (detail->type) {
  case ValueType::Integer: {
    return core::integerClass();
  }
  case ValueType::String: {
    return core::stringClass();
  }
  case ValueType::Class: {
    return detail->holder_.classPointer; // TODO
  }
  case ValueType::Object: {
    return detail->holder_.object->getClass();
  }
  }
  return nullptr; // TODO
}

std::string Value::toString() const {
  std::ostringstream oss;
  switch (detail->type) {
  case ValueType::Boolean:
    oss << std::boolalpha << detail->holder_.booleanValue;
    break;
  case ValueType::Integer:
    oss << detail->holder_.intValue;
    break;
  case ValueType::Double:
    oss << detail->holder_.doubleValue;
    break;
  case ValueType::String:
    oss << detail->holder_.stringValue;
    break;
  case ValueType::Function:
    oss << "<function>";
    break;
  default:
    oss << "<undefined>";
    break;
  }
  return oss.str();
}

Value &Value::operator=(const Value &other) {
  // std::cout << "assign:" << std::endl;
  if (this != &other) {
    detail->mutable_ = other.detail->mutable_;
    detail->type = other.detail->type;
    detail->holder_ = other.detail->holder_;
  }
  return *this;
}

Value &Value::operator=(Value &&other) noexcept {
  // std::cout << "move:" << &(other.detail) << std::endl;
  // std::cout << "type:" << (int)other.type() << std::endl;
  if (this != &other) {
    detail = std::move(other.detail);
    // detail = other.detail;
  }
  // std::cout << "move end" << std::endl;
  return *this;
}

bool Value::operator==(const Value &other) const {
  if (detail->type != other.detail->type) {
    return false;
  }
  switch (detail->type) {
  case ValueType::Boolean:
    return detail->holder_.booleanValue == other.detail->holder_.booleanValue;
  case ValueType::Integer:
    return detail->holder_.intValue == other.detail->holder_.intValue;
  case ValueType::Double:
    return detail->holder_.doubleValue == other.detail->holder_.doubleValue;
  case ValueType::String:
    return detail->holder_.stringValue == other.detail->holder_.stringValue;
  case ValueType::Function:
    return detail->holder_.functionPointer ==
           other.detail->holder_.functionPointer;
  default:
    return true;
  }
}

std::ostream &operator<<(std::ostream &os, const Value &value) {
  switch (value.type()) {
  case ValueType::Boolean:
    os << (value.boolValue() ? "true" : "false");
    break;
  case ValueType::Integer:
    os << value.intValue();
    break;
  case ValueType::Double:
    os << value.doubleValue();
    break;
  case ValueType::String:
    os << value.stringValue();
    break;
  case ValueType::Function:
    os << "<function>" << value.functionValue()->name();
    break;
  case ValueType::Class:  {
    os << "Class"; 
    break;
  case ValueType::Object: {
    os << "Object";
    break;
  }
  }
  default:
    os << "<unknown>";
    break;
  }
  return os;
}

bool Value::operator!=(const Value &other) const { return !(*this == other); }

Value &Value::nil() {
  static Value v;
  return v;
}

// bool Value::isMutable() const {
//     return detail->mutable_;
// }

// void Value::setMutable(bool mutableValue) {
//     detail->mutable_ = mutableValue;
// }
} // namespace kestrel