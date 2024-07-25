cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS YES)
set(PYTHON_SITE_PACKAGE_DIR "/home/appliedai/v/.local/micromamba/envs/dfp/lib/python3.10/site-packages")
set(PYTHON_SITE_PACKAGE_DIR "/Users/khang/.local/micromamba/envs/ml/lib/python3.11/site-packages")

project(mylib)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)
find_package(Arrow REQUIRED)
find_package(Parquet REQUIRED)

# find_package(PkgConfig REQUIRED) # [this works]
# pkg_check_modules(Arrow REQUIRED arrow-python) # [this doesn't work]

pybind11_add_module(mylib mylib.cpp)
target_include_directories(mylib PRIVATE ${PYTHON_INCLUDE_DIRS})
target_include_directories(mylib PRIVATE ${PYTHON_SITE_PACKAGE_DIR}/pyarrow/include)
target_link_libraries(mylib ${PYTHON_LIBRARIES})
target_link_libraries(mylib PRIVATE Arrow::arrow_shared)
target_link_libraries(mylib PRIVATE Parquet::parquet_shared)
target_link_libraries(mylib PRIVATE ${PYTHON_SITE_PACKAGE_DIR}/pyarrow/libarrow_python${CMAKE_SHARED_LIBRARY_SUFFIX})

# --

