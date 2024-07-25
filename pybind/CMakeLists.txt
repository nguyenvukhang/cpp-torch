cmake_minimum_required(VERSION 3.22)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS YES)
set(PYTHON_SITE_PACKAGE_DIR "/home/appliedai/v/.local/micromamba/envs/dfp/lib/python3.10/site-packages")
set(PYTHON_SITE_PACKAGE_DIR "/Users/khang/.local/micromamba/envs/ml/lib/python3.11/site-packages")

project(window_cpp)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)
find_package(Arrow REQUIRED)
find_package(ArrowDataset REQUIRED)
find_package(Parquet REQUIRED)

pybind11_add_module(window_cpp src/lib.cc src/parquet.cc)
target_include_directories(window_cpp PRIVATE ${PYTHON_INCLUDE_DIRS})
target_include_directories(window_cpp PRIVATE ${PYTHON_SITE_PACKAGE_DIR}/pyarrow/include)
target_link_libraries(window_cpp ${PYTHON_LIBRARIES})
target_link_libraries(window_cpp PRIVATE Arrow::arrow_shared)
target_link_libraries(window_cpp PRIVATE ArrowDataset::arrow_dataset_shared)
target_link_libraries(window_cpp PRIVATE Parquet::parquet_shared)
target_link_libraries(window_cpp PRIVATE ${PYTHON_SITE_PACKAGE_DIR}/pyarrow/libarrow_python${CMAKE_SHARED_LIBRARY_SUFFIX})
