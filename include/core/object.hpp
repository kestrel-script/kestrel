#pragma once

#include <unordered_map>
#include <map>

#include "class.hpp"

namespace kestrel {

class Value;
namespace core {

class Object {
public:
    Value& getAttribute(const std::string& name);

    void setClass(Class* cls) {
        this->cls = cls;
    }

    Class* getClass() {
        return this->cls;
    }

private:
    // uint64_t id;
    Class* cls;
    std::map<std::string, Value> attrs; // TODO
};

}
}