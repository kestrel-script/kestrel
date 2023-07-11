#pragma once

#include <memory>

namespace kestrel {
class Compiler;
};

namespace kestrel {

// using ir::Compiler;

struct Statement {
  virtual void print() = 0;

  virtual void prepare(Compiler &compiler) {
    // throw std::runtime_error("prepare not implemented");
  }

  virtual void evaluate(Compiler &compiler) {
    throw std::runtime_error("evaluate not implemented");
  }

  virtual ~Statement() = default; // TODO override all sublclasses;
};


} // namespace kestrel