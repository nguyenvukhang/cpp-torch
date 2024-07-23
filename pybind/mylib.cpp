#include <arrow/api.h>
#include <arrow/csv/api.h>
#include <arrow/io/api.h>
#include <arrow/ipc/api.h>
#include <arrow/python/pyarrow.h>
#include <parquet/arrow/reader.h>
#include <parquet/arrow/writer.h>
#include <pybind11/pybind11.h>

#include <iostream>

int add(int i, int j) { return i + j; }

arrow::Status _read_parquet(std::shared_ptr<arrow::Table> tbl) {
  std::cout << "Hello from C++!" << std::endl;
  std::shared_ptr<arrow::io::ReadableFile> infile;
  std::unique_ptr<parquet::arrow::FileReader> reader;
  ARROW_ASSIGN_OR_RAISE(
      infile, arrow::io::ReadableFile::Open(
                  "/mnt/shared/datasets/backblaze/parquet/2013-04-10.parquet"));
  std::cout << "Survived assignment!" << std::endl;
  std::cout << "Size: " << infile->GetSize().ValueOrDie() << std::endl;

  ARROW_RETURN_NOT_OK(
      parquet::arrow::OpenFile(infile, arrow::default_memory_pool(), &reader));

  // // Read the table.
  PARQUET_THROW_NOT_OK(reader->ReadTable(&tbl));

  return arrow::Status::OK();
}

PyObject* read_parquet() {
  std::shared_ptr<arrow::Table> tbl;
  arrow::Status x = _read_parquet(tbl);
  return arrow::py::wrap_table(tbl);
}

int test_get() { return 69; }

PYBIND11_MODULE(mylib, m) {
  arrow::py::import_pyarrow();
  m.doc() = "pybind11 example plugin";
  m.def("add", &add, "A function that adds two numbers");
  m.def("read_parquet", &read_parquet);
  m.def("test_get", &test_get);
}

// ImportError
// undefined symbol: parquet::arrow::OpenFile::shared_ptr
// arrow::io::RandomAccessFile::MemoryPool::unique_ptr::FileReader::default_delete
//
// undefined symbol: parquet arrow OpenFile shared_ptr io RandomAccessFile
// MemoryPool unique_ptr FileReader default_delete
