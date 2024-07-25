#include <memory>

template <class T>
class RingBuf {
  int pos_len, neg_len, capacity, ptr;
  std::unique_ptr<T[]> buffer;

  int real_index(int idx) const {
    int v = (ptr + idx) % capacity;
    return v >= 0 ? v : v + capacity;
  }

 public:
  RingBuf(int neg_len, int pos_len)
      : pos_len(pos_len),
        neg_len(neg_len),
        capacity(pos_len + neg_len),
        ptr(-1),
        buffer(std::unique_ptr<T[]>(new T[pos_len + neg_len])) {
  }

  T const& operator[](int idx) const {
    return buffer[real_index(idx)];
  }

  T get(int idx) {
    return buffer[real_index(idx)];
  }

  void push(T item) {
    buffer[ptr = (ptr + 1) % capacity] = item;
  }

  void update(int idx, T item) {
    buffer[real_index(idx)] = item;
  }
};
