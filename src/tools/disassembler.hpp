#pragma once

#include <string>
#include "module.hpp"

namespace kestrel {

class Disassembler {
public:
    void run(Module& module_);
};

}