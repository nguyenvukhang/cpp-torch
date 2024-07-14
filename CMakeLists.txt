cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(tt)

set(CMAKE_PREFIX_PATH /Users/khang/.local/libtorch)

find_package(Torch REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

add_executable(tt main.cpp)
target_link_libraries(tt "${TORCH_LIBRARIES}")
set_property(TARGET tt PROPERTY CXX_STANDARD 17)
