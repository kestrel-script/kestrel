#include "core/classes.hpp"
#include <string>
#include "value.hpp"

namespace kestrel {
namespace core {

static Value length (Value& self, MethodParameter& params) {
    return (int)self.stringValue().size();
};

static Value startsWith(Value& self, MethodParameter& params) {
    std::string& str = self.stringValue();
    if (str.rfind(params.args[0].stringValue(), 0) == 0) {
        return true;
    }
    return false;
};

static Value toUpperCase(Value& self, MethodParameter& params) {
    std::string str = self.stringValue();
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
};

static Class* string = nullptr;
Class* stringClass() {
    static std::once_flag flag;
    std::call_once(flag, [](){ 
        string = new Class();
        string->registerMethod("length", length);
        string->registerMethod("startsWith",startsWith);
        string->registerMethod("toUpperCase", toUpperCase);
    });
    return string;
}

}
}