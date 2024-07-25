#include <arrow/api.h>
#include <arrow/io/api.h>
#include <arrow/python/pyarrow.h>
#include <parquet/arrow/reader.h>
#include <pybind11/pybind11.h>

#include <filesystem>
#include <iostream>

#include "bridge.h"
#include "parquet.h"

template <class T>
class Window {
  int pos_len, neg_len, capacity, ptr;
  std::unique_ptr<T[]> buffer;

  int real_index(int idx) const {
    int v = (ptr + idx) % capacity;
    return v >= 0 ? v : v + capacity;
  }

 public:
  Window(int neg_len, int pos_len)
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
};

std::shared_ptr<arrow::Table> py_read_parquet() {
  return read_parquet("2016-01-01.parquet");
}

void py2c(std::shared_ptr<arrow::Table> tbl) {
  for (auto field : tbl->fields()) {
    std::cout << "* field: " << field->ToString() << std::endl;
  }
}

std::shared_ptr<arrow::Table> run() {
  Window<std::shared_ptr<arrow::Table>> win(7, 30);
#define PUSH(x) win.push(read_parquet(x ".parquet"));
  PUSH("2016-01-01");
  PUSH("2016-01-02");
  PUSH("2016-01-03");
#undef PUSH

  return win[-2];
}

PYBIND11_MODULE(window_cpp, m) {
  arrow::py::import_pyarrow();

  m.doc() = "pybind11 example plugin";
  m.def("read_parquet", &py_read_parquet);
  m.def("run", &run);
  m.def("py2c", &py2c);
}
