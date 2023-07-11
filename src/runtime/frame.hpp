#pragma once

#include <string>
#include <iostream>

#include "module.hpp"

namespace kestrel {

struct Frame {

  Frame(Function& function) : function(&function) {}

  int pc = 0;
  int offset = 0;
  std::vector<Value> locals;

  Function* function;

  void print() {
    // std::cout << "pc:" << pc << std::endl;
    // std::cout << "offset:" << offset << std::endl;
    // // std::cout << "registers:" << registers.size() << std::endl;
  }
};

}