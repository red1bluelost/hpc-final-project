cmake_minimum_required(VERSION 3.22)
project(fp_cpp)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Wextra
        $<$<CONFIG:DEBUG>:-g3>
        $<$<CONFIG:DEBUG>:-Og>
        $<$<CONFIG:RELEASE>:-g0>
        $<$<CONFIG:RELEASE>:-O3>
)

add_executable(matmul tools/matmul/main.cpp)
