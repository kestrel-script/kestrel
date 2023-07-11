#pragma once

#include <memory>
#include <vector>

namespace kestrel {

template <typename T>
class List {
public:
  void append(T& item) {
    items.push_back(item);
  }

  void append(T&& item) {
    items.push_back(item);
  }

  int size() {
    return (int) items.size();
  }

  T& operator [](int pos ) {
    return items[pos];
  }

private: 
  std::vector<T> items;
};

}