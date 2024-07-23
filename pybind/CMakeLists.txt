cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

project(mylib)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)
find_package(Arrow REQUIRED)
find_package(Parquet REQUIRED)

# find_package(PkgConfig REQUIRED) # [this works]
# pkg_check_modules(Arrow REQUIRED arrow-python) # [this doesn't work]

pybind11_add_module(mylib mylib.cpp)
target_link_libraries(mylib PRIVATE Arrow::arrow_shared)
target_link_libraries(mylib PRIVATE Parquet::parquet_shared)
target_include_directories(mylib PRIVATE /home/appliedai/v/.local/micromamba/envs/dfp/lib/python3.10/site-packages/pyarrow/include)
target_link_libraries(mylib PRIVATE /home/appliedai/v/.local/micromamba/envs/dfp/lib/python3.10/site-packages/pyarrow/libarrow_python.so)
