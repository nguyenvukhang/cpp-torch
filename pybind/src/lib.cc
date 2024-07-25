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

namespace ac = arrow::compute;

class Window : public RingBuf<std::shared_ptr<arrow::Table>> {
  const int pos_len, neg_len;

 public:
  Window(int neg_len, int pos_len)
      : RingBuf(neg_len + pos_len), pos_len(pos_len), neg_len(neg_len) {
  }

  void push(std::shared_ptr<arrow::Table> tbl) {
    RingBuf::push(tbl->CombineChunks().MoveValueUnsafe());
    online_labelling();
  }

  void online_labelling() {
    std::shared_ptr<arrow::Table> today = RingBuf::operator[](0);
    // https://arrow.apache.org/docs/cpp/compute.html
    arrow::Datum m_failed = arrow::compute::CallFunction(
                                "greater", {today->GetColumnByName("failure"),
                                            arrow::MakeScalar(0)})
                                .MoveValueUnsafe();
    std::shared_ptr<arrow::ChunkedArray> failed_serial_numbers =
        ac::CallFunction("filter", {today, m_failed})
            .ValueUnsafe()
            .table()
            ->GetColumnByName("serial_number");

    ac::SetLookupOptions opts = ac::SetLookupOptions(failed_serial_numbers);

    std::shared_ptr<arrow::Table> tbl;
    for (int i = 1; i < capacity; i++) {
      if ((tbl = RingBuf::operator[](i)) == NULL) continue;
      arrow::Datum mask =
          ac::IsIn(tbl->GetColumnByName("serial_number"), opts).ValueUnsafe();
      // If it's in the list of failed serial numbers, update the value to 1
      // else, keep the current value.

      tbl = ac::CallFunction("if_else", {mask, arrow::MakeScalar(1),
                                         tbl->GetColumnByName("failure")})
                ->table();
      std::cout << "-------------->" << std::endl;
      update(i, tbl);

      // // arrow::Datum mask2 = ac::CallFunction("equal", )
      //
      // tbl = ac::Filter(tbl, mask)->table();
      // std::cout << tbl->GetColumnByName("serial_number")->ToString()
      //           << std::endl;
    }
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
  for (int i = 0; i < 50; i++) {
    auto tbl = read_parquet((std::string(DATES[i]) + ".parquet").c_str());
    win.push(tbl);
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
