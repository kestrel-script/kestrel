#pragma once

#include <map>
#include <string>

namespace kestrel {

enum class Opcode : uint8_t {
  NoOP = 0,
  Add,
  Subtract,
  Multiply,
  Divide,
  Move,
  Branch,
  BranchTrue,
  BranchFalse,

  Equals,
  LessThan,
  GreaterThan,

  Duplicate,
  LoadBoolean, //TODO LoadTrue,LoadFalse
  LoadInteger,
  LoadConstant,
  LoadName,
  LoadLocal,
  LoadGlobal,
  LoadGlobalFromPool,
  LoadNil,
  Import,

  GetItem,

  Store,

  Call,
  Dispatch,

  Return, // Exit from the current function and return the value on the top of
          // the stack.

};

inline std::string toString(Opcode code) {
#define REGISTER_CODE(code)                                                    \
  { Opcode::code, #code }

  static std::map<Opcode, std::string> codemap = {
      {Opcode::NoOP, "NoOP"},
      REGISTER_CODE(Add),
      REGISTER_CODE(Subtract),
      REGISTER_CODE(Multiply),
      REGISTER_CODE(Divide),
      REGISTER_CODE(Move),
      REGISTER_CODE(Branch),
      REGISTER_CODE(BranchTrue),
      REGISTER_CODE(BranchFalse),

      REGISTER_CODE(LessThan),
      REGISTER_CODE(GreaterThan),

      REGISTER_CODE(LoadBoolean),
      REGISTER_CODE(LoadInteger),
      REGISTER_CODE(LoadConstant),
      REGISTER_CODE(LoadGlobal),
      REGISTER_CODE(LoadGlobalFromPool), // TODO
      REGISTER_CODE(LoadNil),

      REGISTER_CODE(Call),
      REGISTER_CODE(Return),
  };

#undef REGISTER_CODE
  if (codemap.count(code) > 0) {
    return codemap[code];
  }
  return "unknown";
}

} // namespace kestrel
