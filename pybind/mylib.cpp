#include <Python.h>
#include <arrow/api.h>
#include <arrow/c/abi.h>
#include <arrow/c/bridge.h>
#include <arrow/io/api.h>
#include <arrow/python/pyarrow.h>
#include <arrow/table.h>
#include <arrow/table_builder.h>
#include <parquet/arrow/reader.h>
#include <pybind11/pybind11.h>

#include <iostream>

// =============================================================================

namespace pybind11 {
namespace detail {
template <typename TableType>
struct gen_type_caster {
 public:
  PYBIND11_TYPE_CASTER(std::shared_ptr<TableType>, _("pyarrow::Table"));
  // C++ -> Python
  static handle cast(std::shared_ptr<TableType> src, return_value_policy,
                     handle) {
    return arrow::py::wrap_table(src);
  }
};
template <>
struct type_caster<std::shared_ptr<arrow::Table>>
    : public gen_type_caster<arrow::Table> {};
}  // namespace detail
}  // namespace pybind11

// =============================================================================

int add(int i, int j) { return i + j; }

// const char* PQ_PATH =
// "/mnt/shared/datasets/backblaze/parquet/2013-04-10.parquet";
const char* PQ_PATH =
    "/Users/khang/.local/data/backblaze/parquets/2013-04-10.parquet";

arrow::Status _read_parquet(std::shared_ptr<arrow::Table>* tbl) {
  std::cout << "Hello from C++!" << std::endl;
  std::shared_ptr<arrow::io::ReadableFile> infile;
  std::unique_ptr<parquet::arrow::FileReader> reader;
  ARROW_ASSIGN_OR_RAISE(infile, arrow::io::ReadableFile::Open(PQ_PATH));
  std::cout << "Survived assignment!" << std::endl;
  std::cout << "Size: " << infile->GetSize().ValueOrDie() << std::endl;

  ARROW_RETURN_NOT_OK(
      parquet::arrow::OpenFile(infile, arrow::default_memory_pool(), &reader));

  // // Read the table.
  PARQUET_THROW_NOT_OK(reader->ReadTable(tbl));
  std::cout << "Survived read!" << std::endl;
  // for (auto field : tbl->fields()) {
  //   std::cout << "Field: " << field->ToString() << std::endl;
  // }

  return arrow::Status::OK();
}

// std::shared_ptr<arrow::Table> read_parquet() {
std::shared_ptr<arrow::Table> read_parquet() {
  std::shared_ptr<arrow::Table> tbl;
  arrow::Status x = _read_parquet(&tbl);
  int i = 0;
  for (auto field : tbl->fields()) {
    if (i++ > 5) break;
    std::cout << "Field: " << field->ToString() << std::endl;
  }
  std::cout << "--- End of fields ---" << std::endl;
  return tbl;
  // PyObject* wrapped = arrow::py::wrap_table(tbl);
  // std::cout << "Survived wrap!" << std::endl;
  // return wrapped;
  // std::cout << "Wrapping table!" << std::endl;
  // PyObject* y = arrow::py::wrap_table(tbl);
  // std::cout << "Wrapped table!" << std::endl;
  // return y;
}

int test_get() { return 69; }

PYBIND11_MODULE(mylib, m) {
  Py_Initialize();
  if (!Py_IsInitialized()) {
    std::cerr << "Python initialization failed!" << std::endl;
    exit(1);
  }
  std::cerr << "Python interpreter initialized!" << std::endl;
  arrow::py::import_pyarrow();
  std::cout << "Imported <pyarrow> !" << std::endl;

  m.doc() = "pybind11 example plugin";
  m.def("add", &add, "A function that adds two numbers");
  m.def("read_parquet", &read_parquet,
        pybind11::call_guard<pybind11::gil_scoped_release>());
  // bool scoped = true;
  // if (scoped) {
  //   m.def("read_parquet", &read_parquet,
  //         pybind11::call_guard<pybind11::gil_scoped_release>());
  // } else {
  //   m.def("read_parquet", &read_parquet);
  // }
  m.def("test_get", &test_get);
}

// ImportError
// undefined symbol: parquet::arrow::OpenFile::shared_ptr
// arrow::io::RandomAccessFile::MemoryPool::unique_ptr::FileReader::default_delete
//
// undefined symbol: parquet arrow OpenFile shared_ptr io RandomAccessFile
// MemoryPool unique_ptr FileReader default_delete
