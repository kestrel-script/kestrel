#include "object.hpp"
#include "value.hpp"

namespace kestrel {
namespace core {

Value& Object::getAttribute(const std::string& name) {
    if (attrs.find(name) != attrs.end()) {
        return attrs[name];
    }
    return Value::nil();
}

}
}