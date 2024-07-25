#include "parquet.h"

#include <arrow/api.h>
#include <arrow/compute/api.h>
#include <arrow/dataset/api.h>
#include <arrow/datum.h>
#include <arrow/io/api.h>

#include <ctime>

#include "arrow/compute/api_vector.h"

namespace fs = std::filesystem;
namespace ac = arrow::compute;

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
  ac::ScalarAggregateOptions scalar_aggregate_options;
  scalar_aggregate_options.skip_nulls = false;

  arrow::Datum min_max =
      ac::CallFunction("min_max", {tbl->GetColumnByName("capacity_bytes")},
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
      ac::CallFunction("add",
                       {tbl->GetColumnByName("capacity_bytes"), increment})
          .ValueUnsafe();
  std::cout << datum.ToString() << std::endl;
}

void cum_sum(const std::shared_ptr<arrow::Table>& tbl) {
  ac::CumulativeOptions opts;
  arrow::Datum datum =
      ac::CallFunction("cumulative_sum",
                       {tbl->GetColumnByName("capacity_bytes")}, &opts)
          .ValueUnsafe();
  std::cout << datum.ToString() << std::endl;
}

arrow::Datum equaller(const std::shared_ptr<arrow::Table>& tbl) {
  return ac::CallFunction("equal", {tbl->GetColumnByName("model"),
                                    arrow::MakeScalar("ST8000DM002")})
      .ValueUnsafe();
}

std::shared_ptr<arrow::Table> filter_model(
    const std::shared_ptr<arrow::Table>& tbl) {
  arrow::Datum filt = equaller(tbl);
  auto x = ac::Filter(tbl, filt).ValueUnsafe();
  arrow::Datum datum =
      ac::CallFunction("filter", {tbl->GetColumnByName("model"), filt})
          .ValueUnsafe();
  return x.table();
}

std::shared_ptr<arrow::Table> failures(
    const std::shared_ptr<arrow::Table>& tbl) {
  arrow::Datum mask =
      ac::CallFunction("greater",
                       {tbl->GetColumnByName("failure"), arrow::MakeScalar(0)})
          .ValueUnsafe();
  return ac::CallFunction("filter", {tbl, mask}).ValueUnsafe().table();
}

std::shared_ptr<arrow::Table> my_model(std::shared_ptr<arrow::Table>& tbl) {
  std::shared_ptr<arrow::Scalar> model_str = arrow::MakeScalar("ST8000DM002");
  arrow::Datum mask =
      ac::CallFunction("equal", {tbl->GetColumnByName("model"), model_str})
          .ValueUnsafe();
  return ac::Filter(tbl, mask).ValueUnsafe().table();
}

std::shared_ptr<arrow::Table> read_parquet(const char* filename) {
  std::shared_ptr<arrow::Table> tbl;
  arrow::Status x = read_parquet(tbl, filename);
  return my_model(tbl);
}
