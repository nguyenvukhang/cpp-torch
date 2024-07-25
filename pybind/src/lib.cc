#include <arrow/api.h>
#include <arrow/compute/api.h>
#include <arrow/io/api.h>
#include <arrow/python/pyarrow.h>
#include <parquet/arrow/reader.h>
#include <pybind11/pybind11.h>

#include <ctime>
#include <filesystem>
#include <iostream>

#include "bridge.h"
#include "dates2018.h"
#include "parquet.h"
#include "ringbuf.h"

class Window : public RingBuf<std::shared_ptr<arrow::Table>> {
  const int pos_len, neg_len;

 public:
  Window(int neg_len, int pos_len)
      : RingBuf(neg_len + pos_len), pos_len(pos_len), neg_len(neg_len) {
  }

  void push(std::shared_ptr<arrow::Table> tbl) {
    // https://arrow.apache.org/docs/cpp/compute.html
    arrow::Datum d =
        arrow::compute::CallFunction(
            "greater", {tbl->GetColumnByName("failure"), arrow::MakeScalar(0)})
            .ValueUnsafe();
    // return arrow::compute::CallFunction(
    //            "equal",
    //            {tbl->GetColumnByName("model"),
    //            arrow::MakeScalar("ST8000DM002")})
    //     .ValueUnsafe();
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
  for (int i = 0; i < 40; i++) {
    win.push(read_parquet((std::string(DATES[i]) + ".parquet").c_str()));
  }

  return win[-2];
}

PYBIND11_MODULE(window_cpp, m) {
  arrow::py::import_pyarrow();

  m.doc() = "pybind11 example plugin";
  m.def("read_parquet", &py_read_parquet);
  m.def("run", &run);
  m.def("py2c", &py2c);
}
