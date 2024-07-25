#include "parquet.h"

#include <arrow/api.h>
#include <arrow/compute/api.h>
#include <arrow/dataset/api.h>
#include <arrow/datum.h>
#include <arrow/io/api.h>

#include "arrow/compute/api_vector.h"

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

void min_maxer(const std::shared_ptr<arrow::Table>& tbl) {
  arrow::compute::ScalarAggregateOptions scalar_aggregate_options;
  scalar_aggregate_options.skip_nulls = false;

  arrow::Datum min_max =
      arrow::compute::CallFunction("min_max",
                                   {tbl->GetColumnByName("capacity_bytes")},
                                   &scalar_aggregate_options)
          .ValueUnsafe();

  std::shared_ptr<arrow::Scalar> min_value, max_value;
  min_value = min_max.scalar_as<arrow::StructScalar>().value[0];
  max_value = min_max.scalar_as<arrow::StructScalar>().value[1];
  printf("Max: %s\n", max_value->ToString().c_str());
  printf("Min: %s\n", min_value->ToString().c_str());
}

void adder(const std::shared_ptr<arrow::Table>& tbl) {
  std::shared_ptr<arrow::Scalar> increment = arrow::MakeScalar(10);
  arrow::Datum datum =
      arrow::compute::CallFunction(
          "add", {tbl->GetColumnByName("capacity_bytes"), increment})
          .ValueUnsafe();
  std::cout << datum.ToString() << std::endl;
}

void cum_sum(const std::shared_ptr<arrow::Table>& tbl) {
  arrow::compute::CumulativeOptions opts;
  arrow::Datum datum =
      arrow::compute::CallFunction(
          "cumulative_sum", {tbl->GetColumnByName("capacity_bytes")}, &opts)
          .ValueUnsafe();
  std::cout << datum.ToString() << std::endl;
}

std::shared_ptr<arrow::Table> my_model(std::shared_ptr<arrow::Table>& tbl) {
  cum_sum(tbl);
  std::shared_ptr<arrow::dataset::ScanOptions> options =
      std::make_shared<arrow::dataset::ScanOptions>();
  options->filter =
      arrow::compute::equal(arrow::compute::field_ref("model"),
                            arrow::compute::literal("ST8000DM002"));
  return arrow::dataset::ScannerBuilder(
             std::make_shared<arrow::dataset::InMemoryDataset>(tbl), options)
      .Finish()
      .ValueUnsafe()
      ->ToTable()
      .ValueUnsafe();
}

std::shared_ptr<arrow::Table> read_parquet(const char* filename) {
  std::shared_ptr<arrow::Table> tbl;
  arrow::Status x = read_parquet(tbl, filename);
  return my_model(tbl);
}
