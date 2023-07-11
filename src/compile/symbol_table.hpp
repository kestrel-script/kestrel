#pragma once

#include <algorithm>
#include <string>
#include <vector>

namespace kestrel {

struct Local {
  std::string name;
  int level;
};

class SymbolTable {
public:
  void enterScope() { level++; }
  void exitScope() { level--; }
  int findSymbol(const std::string &name) {
    for (int i = (int)locals.size() - 1; i >= 0; i--) {
      if (locals[i].name == name) {
        return i;
      }
    }
    return -1;
  }
  int addSymbol(const std::string &name) {
    locals.push_back({name, level});
    maxSlots_ = std::max(maxSlots_, (int)locals.size());
    return locals.size() - 1;
  }
  bool checkSymbol(const std::string &name) {
    for (int i = (int)locals.size() - 1; i >= 0; i--) {
      if (locals[i].level == level && locals[i].name == name) {
        return true;
      }
    }
    return false;
  }

  int maxSlots() { return maxSlots_; }

private:
  std::vector<Local> locals;
  int level = 0;
  int maxSlots_;
};

} // namespace kestrel
