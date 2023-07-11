#pragma once

#include <string>
#include <iostream>
#include "module.hpp"

#include "runtime/frame.hpp"

#include "runtime/array.hpp"

namespace kestrel {

class InterpreterImpl;
class Interpreter {
public:
    void run(Module& module, Function& function);

    // int framePointer = 0;
    // Array<Frame> frames;
    // Frame& currentFrame = frames.back();
};

}