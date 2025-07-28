#ifndef UTILS_H
#define UTILS_H
#include <cassert>
#include <iostream>

namespace cpu_sim {
  using u32 = unsigned int;
  using u8 = unsigned char;
  using i32 = int;

  constexpr int kRegSize = 32;

  constexpr int kRSSize = 32;

  constexpr int kROBSize = 32;

  constexpr int kLSBSize = 32;

  extern bool halt;
  extern u32 now_pc;
  extern u32 next_pc;
  extern u32 exec_pc;

  inline void upd_pc() {
    now_pc = next_pc;
  }

  template<class T, int Size>
  class Arr {
  public:
    Arr() = default;

    ~Arr() = default;

    Arr(const Arr &other) {
      for (int i = 0; i < Size; i++) {
        data_[i] = other.data_[i];
        flag_[i] = other.flag_[i];
      }
      this->size_ = other.size_;
    }

    Arr &operator=(const Arr &other) {
      if (this == &other) return *this;
      for (int i = 0; i < Size; i++) {
        data_[i] = other.data_[i];
        flag_[i] = other.flag_[i];
      }
      this->size_ = other.size_;
      return *this;
    }

    T &operator[](int index) {
      if (!flag_[index]) {
        std::cerr << "error in Arr: invalid index!" << std::endl;
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
        flag_[i] = false;
      }
    }

  private:
    T data_[Size]{};
    bool flag_[Size]{};
    int size_ = 0;
  };

  template<class T, int Size>
  class CirQue {
  public:
    CirQue() = default;

    ~CirQue() = default;

    CirQue(const CirQue &other) {
      for (int i = 0; i < Size; i++) {
        data_[i] = other.data_[i];
      }
      this->size_ = other.size_;
      this->head_ = other.head_;
      this->tail_ = other.tail_;
    }

    CirQue &operator=(const CirQue &other) {
      if (this == &other) return *this;
      for (int i = 0; i < Size; i++) {
        data_[i] = other.data_[i];
      }
      this->size_ = other.size_;
      this->head_ = other.head_;
      this->tail_ = other.tail_;
      return *this;
    }

    int head() const {
      return head_;
    }

    int tail() const {
      return tail_;
    }

    int size() const {
      return size_;
    }

    T &front() {
      if (empty()) {
        std::cerr << "error front: empty CirQue" << std::endl;
        assert(false);
      }
      return data_[head_];
    }

    void push(const T &item) {
      if (full()) {
        std::cerr << "error push: full CirQue" << std::endl;
        assert(false);
      }
      data_[tail_] = item;
      tail_ = (tail_ + 1) % Size;
      size_++;
    }

    void pop() {
      if (empty()) {
        std::cerr << "error pop: empty CirQue" << std::endl;
        assert(false);
      }
      head_ = (head_ + 1) % Size;
      size_--;
    }

    bool full() const {
      return size_ == Size;
    }

    bool empty() const {
      return size_ == 0;
    }

    void clear() {
      head_ = tail_ = size_ = 0;
    }

    T &operator[](int index) {
      if (index < 0 || index >= Size) {
        std::cerr << "error in CirQue: invalid index!" << std::endl;
        assert(false);
      }
      return data_[index];
    }

  private:
    T data_[Size]{};
    int head_ = 0;
    int tail_ = 0;
    int size_ = 0;
  };
}

#endif //UTILS_H
