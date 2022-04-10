cmake_minimum_required(VERSION 3.22)
project(fp_c C)

set(CMAKE_C_STANDARD 11)

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

set(PROJECT_SOURCE_DIR lib)
set(PROJECT_HEADER_DIR include)

include_directories(${PROJECT_HEADER_DIR})



add_executable(fft
        tools/fft/main.c)

add_executable(matmul
        tools/matmul/main.c)
