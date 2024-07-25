#include <iostream>
#include <memory>

template <class T>
class RingBuf {
  const std::unique_ptr<T[]> buffer;
  int ptr;

 public:
  const int capacity;

  int real_index(int idx) const {
    int v = (ptr + idx) % capacity;
    return v >= 0 ? v : v + capacity;
  }

  RingBuf(int capacity)
      : capacity(capacity),
        ptr(-1),
        buffer(std::unique_ptr<T[]>(new T[capacity])) {
  }

  std::vector<T> collect_raw() {
    std::vector<T> v;
    for (int i = 0; i < capacity; v.push_back(buffer[i++]));
    return v;
  }

  T const& operator[](int idx) const {
    return buffer[real_index(idx)];
  }

  void push(T item) {
    buffer[ptr = (ptr + 1) % capacity] = item;
  }

  void update(int idx, T item) {
    buffer[real_index(idx)] = item;
  }
};
