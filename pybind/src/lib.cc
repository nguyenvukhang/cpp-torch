#include <arrow/api.h>
#include <arrow/io/api.h>
#include <arrow/python/pyarrow.h>
#include <parquet/arrow/reader.h>
#include <pybind11/pybind11.h>

#include <filesystem>
#include <iostream>

#include "bridge.h"
#include "parquet.h"
#include "ringbuf.h"

class Window : public RingBuf<std::shared_ptr<arrow::Table>> {
  const int pos_len, neg_len;

 public:
  Window(int neg_len, int pos_len)
      : RingBuf(neg_len + pos_len), pos_len(pos_len), neg_len(neg_len) {
  }

  void push(std::shared_ptr<arrow::Table> tbl) {
    RingBuf::push(tbl);
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
  Window win(7, 30);
#define PUSH(x) win.push(read_parquet(x ".parquet"));
  PUSH("2018-01-01");
  PUSH("2018-01-02");
  PUSH("2018-01-03");
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
