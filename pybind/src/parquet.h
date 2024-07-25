#include <arrow/api.h>
#include <arrow/io/api.h>
#include <parquet/arrow/reader.h>

#include <ctime>
#include <filesystem>
#include <iostream>

std::shared_ptr<arrow::Table> read_parquet(const char *);
