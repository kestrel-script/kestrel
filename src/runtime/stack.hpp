#include <iostream>
#include <vector>

namespace kestrel {

template <typename T>
class Stack {
private:
  std::vector<T> data;

public:
  void push(T& value) { data.push_back(value); }
  void push(T&& value) { data.push_back(value); }

  int size() {
    return (int) data.size();
  }

  T& top() {
    if (data.empty()) {
      throw std::runtime_error("Stack is empty");
    }
    return data.back();
  }

  T pop() {
    if (data.empty()) {
      throw std::runtime_error("Stack is empty");
    }

    T top = data.back();
    data.pop_back();
    return top; // TODO std::move
  }

  void pop(int num) {
    data.erase(data.end() - num, data.end());
  }

  T& operator[](int index) {
    if (index >= 0) {
      if (static_cast<std::vector<int>::size_type>(index) >= data.size()) {
        throw std::runtime_error("Index out of range");
      }
      return data[index];
    } else {
      int top_index = static_cast<int>(data.size()) - 1;
      int real_index = top_index + index + 1;
      if (real_index < 0 ||
          static_cast<std::vector<int>::size_type>(real_index) >= data.size()) {
          throw std::runtime_error("Index out of range");
      }
      return data[real_index];
    }
  }

  void inspect() {
    for (int i = 0; i < data.size(); i++) {
      std::cout << i << " : " << data[i] << std::endl;
    }
  }
};

}
