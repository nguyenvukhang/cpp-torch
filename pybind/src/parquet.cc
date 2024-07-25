#include "parquet.h"

namespace fs = std::filesystem;

// `/mnt/shared/datasets/backblaze/parquet`
const fs::path PQ_DIR = "/Users/khang/.local/data/backblaze/parquets";

arrow::Status read_parquet(std::shared_ptr<arrow::Table>& tbl,
                           const char* filename) {
  std::shared_ptr<arrow::io::ReadableFile> infile;
  std::unique_ptr<parquet::arrow::FileReader> reader;
  ARROW_ASSIGN_OR_RAISE(infile,
                        arrow::io::ReadableFile::Open(PQ_DIR / filename));
  ARROW_RETURN_NOT_OK(
      parquet::arrow::OpenFile(infile, arrow::default_memory_pool(), &reader));
  PARQUET_THROW_NOT_OK(reader->ReadTable(&tbl));
  return arrow::Status::OK();
}

std::shared_ptr<arrow::Table> read_parquet(const char* filename) {
  std::shared_ptr<arrow::Table> tbl;
  arrow::Status x = read_parquet(tbl, filename);
  return tbl;
}
