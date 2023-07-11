#pragma once

#include <map>
#include <memory>
#include <string>

#include "class.hpp"

namespace kestrel {

class Token;

// class Object: public std::enable_shared_from_this<Object> {
//   std::shared_ptr<ClassStatement> klass;
//   std::map<std::string, Value> fields;

// public:
//   Object(std::shared_ptr<ClassStatement> klass);
//   Value get(const Token& name);
//   void set(const Token& name, Value value);
//   std::string toString();
// };

}