#ifndef __WINDOW_CPP_TYPECAST__
#define __WINDOW_CPP_TYPECAST__

#include <arrow/python/pyarrow.h>
#include <pybind11/pybind11.h>

namespace pybind11 {
namespace detail {
template <typename TableType>
struct gen_type_caster {
 public:
  PYBIND11_TYPE_CASTER(std::shared_ptr<TableType>, _("pyarrow::Table"));
  // Python -> C++
  bool load(handle src, bool) {
    if (!arrow::py::is_table(src.ptr())) return false;
    arrow::Result<std::shared_ptr<arrow::Table>> result =
        arrow::py::unwrap_table(src.ptr());
    if (!result.ok()) return false;
    value = std::static_pointer_cast<TableType>(result.ValueOrDie());
    return true;
  }
  // C++ -> Python
  static handle cast(std::shared_ptr<TableType> src, return_value_policy,
                     handle) {
    if (src == NULL) {
      return pybind11::none();
    }
    return arrow::py::wrap_table(src);
  }
};
template <>
struct type_caster<std::shared_ptr<arrow::Table>>
    : public gen_type_caster<arrow::Table> {};
}  // namespace detail
}  // namespace pybind11

#endif
