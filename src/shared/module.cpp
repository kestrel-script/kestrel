// #pragma once

// #include <map>
// #include <memory>
// #include <string>
// #include <vector>

// #include "value.hpp"
// #include "function.hpp"

// #include "instruction_array.hpp"

// namespace kestrel {

// class Module {
// public:
//   Value &getGlobal(const std::string &name) { return globals_[name]; }

//   void setGlobal(const std::string &name, Value v) { globals_[name] = v; }

//   int putConstant(Value& value) { // TODO rename;
//     for (int i = 0; i < constants.size(); i++) {
//       if (constants[i] == value) {
//         return i;
//       }
//     }
//     int index = constants.size();
//     constants.push_back(value);
//     return index;
//   }

//   Value &getConstant(int index) { return constants[index]; }

//   InstructionArray instructions;

// public:
//   // Value initializer;
//   Function initializer_;
//   std::vector<Value> constants;
//   std::vector<std::string> names;
//   std::map<std::string, Value> globals_;
// };

// } // namespace kestrel
