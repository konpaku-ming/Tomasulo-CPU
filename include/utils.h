#ifndef UTILS_H
#define UTILS_H
#include <cassert>
#include <iostream>

using u32 = unsigned int;
using u8 = unsigned char;
using i32 = int;

constexpr int kRegSize = 32;

constexpr int kRSSize = 32;

template<class T, int Size>
class Arr {
public:
  Arr() = default;

  Arr(const Arr &other) {
    for (int i = 0; i < Size; i++) {
      data_[i] = other.data_[i];
    }
    this->size_ = other.size_;
  }

  Arr &operator=(const Arr &other) {
    if (this == &other) return *this;
    for (int i = 0; i < Size; i++) {
      data_[i] = other.data_[i];
    }
    this->size_ = other.size_;
    return *this;
  }

  T &operator[](int index) {
    if (!flag_[index]) {
      std::cerr << "invalid index!" << std::endl;
      assert(false);
    }
    return data_[index];
  }

  int size() const {
    return size_;
  }

  bool full() const {
    return size_ == Size;
  }

  bool empty() const {
    return size_ == 0;
  }

  bool exist(int pos) {
    return flag_[pos];
  }

  void push(const T &val) {
    if (full()) {
      std::cerr << "failed to push data into a full Arr!" << std::endl;
      assert(false);
    }

    int pos = -1;
    for (int i = 0; i < Size; i++) {
      if (!flag_[i]) {
        pos = i;
        break;
      }
    }

    if (pos == -1) {
      std::cerr << "confused: cannot find empty position" << std::endl;
      assert(false);
    }

    size_++;
    flag_[pos] = true;
    data_[pos] = val;
  }

  void remove(int pos) {
    if (empty()) {
      std::cerr << "failed to remove data from a empty Arr!" << std::endl;
      assert(false);
    }
    if (!flag_[pos]) {
      std::cerr << "cannot remove empty data" << std::endl;
      assert(false);
    }
    size_--;
    flag_[pos] = false;
  }

  void clear() {
    size_ = 0;
    for (int i = 0; i < Size; i++) {
      flag_ = false;
    }
  }

private:
  T data_[Size]{};
  bool flag_[Size]{};
  int size_ = 0;
};

#endif //UTILS_H
