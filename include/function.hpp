#pragma once

#include <functional>
#include <memory>

#include "core/object.hpp"
#include "instruction_array.hpp"
#include "value.hpp"

namespace kestrel {

using ForeignFunction = std::function<Value(std::vector<Value> &args)>;

enum FunctionType { Native = 0, Foreign = 1 };

class Function : core::Object {
public:
  Function();
  Function(ForeignFunction f);
  Function(InstructionArray &instructions);
  ~Function();

  Function(const Function &other);
  Function &operator=(const Function &other);

  Function(Function &&other) noexcept;
  Function &operator=(Function &&other) noexcept;

  std::string name() const;
  void setName(const std::string &name);

  FunctionType type() const;
  ForeignFunction &foreignFunction();
  std::string toString() const;

  InstructionArray &instructions();

  void setArity(size_t arity);
  int arity() const;

  int maxSlots() const;
  void setMaxSlots(int size);

private:
  class Detail;
  std::unique_ptr<Detail> detail;
};

} // namespace kestrel