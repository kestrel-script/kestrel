#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

#include "value.hpp"
#include "function.hpp"

#include "instruction_array.hpp"

namespace kestrel {

class Module {
public:
  Value &getGlobal(const std::string &name) { return globals_[name]; }

  void setGlobal(const std::string &name, Value& v) { globals_[name] = v; }

  void setGlobal(const std::string &name, Value&& v) { globals_[name] = v; }

  int putConstant(Value& value) { // TODO rename;
    for (int i = 0; i < constants.size(); i++) {
      if (constants[i] == value) {
        return i;
      }
    }
    int index = (int) constants.size();
    constants.push_back(value);
    return index;
  }

  Value &getConstant(int index) { return constants[index]; }

  bool hasGlobal(const std::string& name) {
    return globals_.find(name) != globals_.end();
  }

  int nameIndex(const std::string& name) {
    for (int i = 0; i < names.size(); i++) {
      if (name == names[i]) {
        return i;
      }
    }
    names.push_back(name);
    return names.size() - 1;
  }

  InstructionArray instructions;

public:
  // Value initializer;
  Function initializer_;
  std::vector<Value> constants;
  std::vector<std::string> names;
  std::unordered_map<std::string, Value> globals_;
};

} // namespace kestrel
