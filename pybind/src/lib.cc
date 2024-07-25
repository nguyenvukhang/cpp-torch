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

class Window {
  RingBuf<std::shared_ptr<arrow::Table>> rb;
  const int pos_len, neg_len;

  std::shared_ptr<arrow::Table> standardize(std::shared_ptr<arrow::Table> tbl) {
    std::shared_ptr<arrow::DataType> f32 = arrow::float32();
    for (int i = 0; i < tbl->schema()->num_fields(); i++) {
      std::shared_ptr<arrow::Field> field = tbl->schema()->field(i);
      std::string_view name = field->name();
      if (name.starts_with("smart_") || name == "failure") {
        if (field->type() == f32) continue;
        arrow::Datum new_data =
            ac::Cast(tbl->column(i), f32, ac::CastOptions::Unsafe())
                .MoveValueUnsafe();
        std::shared_ptr<arrow::Field> new_field =
            std::make_shared<arrow::Field>(field->name(), f32);
        tbl = tbl->SetColumn(i, new_field, new_data.chunked_array())
                  .MoveValueUnsafe();
      }
    }
    return tbl;
  }

 public:
  Window(int neg_len, int pos_len)
      : rb(neg_len + pos_len), pos_len(pos_len), neg_len(neg_len) {
  }

  std::shared_ptr<arrow::Table> get_all() {
    std::vector<std::shared_ptr<arrow::Table>> tbls;
    for (int i = rb.capacity - 1; i >= 0; i--)
      if (rb[-i] != NULL) tbls.push_back(rb[-i]);
    auto opts = arrow::ConcatenateTablesOptions();
    opts.unify_schemas = true;
    opts.field_merge_options.promote_integer_to_float = true;
    return arrow::ConcatenateTables(tbls, opts).MoveValueUnsafe();
  }

  void push(std::shared_ptr<arrow::Table> tbl) {
    rb.push(standardize(tbl)->CombineChunks().MoveValueUnsafe());
    online_labelling();
  }

  void online_labelling() {
    std::shared_ptr<arrow::Table> today = rb[0];
    // https://arrow.apache.org/docs/cpp/compute.html
    arrow::Datum m_failed = arrow::compute::CallFunction(
                                "greater", {today->GetColumnByName("failure"),
                                            arrow::MakeScalar(0)})
                                .MoveValueUnsafe();
    std::shared_ptr<arrow::ChunkedArray> failed_serial_numbers =
        ac::CallFunction("filter", {today, m_failed})
            .MoveValueUnsafe()
            .table()
            ->GetColumnByName("serial_number");

    ac::SetLookupOptions opts = ac::SetLookupOptions(failed_serial_numbers);

    std::shared_ptr<arrow::Table> tbl;
    for (int i = 1; i < pos_len; i++) {
      if ((tbl = rb[-i]) == NULL) continue;
      arrow::Datum mask = ac::IsIn(tbl->GetColumnByName("serial_number"), opts)
                              .MoveValueUnsafe();

      std::shared_ptr<arrow::ChunkedArray> label_col =
          tbl->GetColumnByName("failure");
      arrow::Datum x =
          ac::CallFunction("if_else", {mask, arrow::MakeScalar(1), label_col})
              .MoveValueUnsafe();

      bool set = 0;
      for (int j = 0; j < tbl->schema()->num_fields(); j++) {
        const std::shared_ptr<arrow::Field> f = tbl->schema()->field(j);
        if (f->name() == "failure") {
          tbl = tbl->SetColumn(j, f, x.chunked_array()).MoveValueUnsafe();
          set |= 1;
          break;
        }
      }
      if (!set) exit(1);
      rb.update(-i, tbl);
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
  for (int i = 0; i < 75; i++) {
    auto tbl = read_parquet((std::string(DATES[i]) + ".parquet").c_str());
    win.push(tbl);
  }
  return win.get_all();
}

PYBIND11_MODULE(window_cpp, m) {
  arrow::py::import_pyarrow();

  m.doc() = "pybind11 example plugin";
  m.def("read_parquet", &py_read_parquet);
  m.def("run", &run);
  m.def("py2c", &py2c);
}
