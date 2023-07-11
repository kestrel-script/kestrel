#pragma once

#include <string>
#include <vector>
#include "module.hpp"

namespace kestrel {

template <typename T>
struct Array {

  void emplace(T&& args... ) {
    array.emplace(array.end(), args);
  }

  T& at(int index) {
    if (index >= 0) {
      return array[index];
    }
    return array[array.size() + index];
  }

  void pop_back() {
    array.erase(array.end() - 1);
  }

  bool empty() {
    return array.empty();
  }

  T& back() {
    return at(-1);
  }

  int size() {
    return array.size();
  }

  std::vector<T> array;

};

}