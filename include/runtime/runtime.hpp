#pragma once

#include <string>
#include <memory>
#include "module.hpp"
#include "value.hpp"

namespace kestrel {

class Runtime {
public:
    Runtime();
    ~Runtime();

    Module& defineModule(const std::string& name);

    Value run(Module& module, Function& function);

private:
    struct Detail;
    std::unique_ptr<Detail> detail;
};

}