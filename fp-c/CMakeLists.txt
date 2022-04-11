cmake_minimum_required(VERSION 3.22)
project(fp_c C)

set(CMAKE_C_STANDARD 11)

add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        $<$<CONFIG:DEBUG>:-g3>
        $<$<CONFIG:DEBUG>:-Og>
        $<$<CONFIG:RELEASE>:-g0>
        $<$<CONFIG:RELEASE>:-O3>
        -fsanitize=address
)

add_link_options(-fsanitize=address)


set(PROJECT_SOURCE_DIR lib)
set(PROJECT_HEADER_DIR include)

include_directories(${PROJECT_HEADER_DIR})

set(FFT_SOURCE_FILES)
set(FFT_HEADER_FILES)

set(MATRIX_SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/matrix/matrix.c)
set(MATRIX_HEADER_FILES
        ${PROJECT_HEADER_DIR}/matrix/matrix.h)

set(UTIL_SOURCE_FILES)
set(UTIL_HEADER_FILES)

add_executable(fft
        tools/fft/main.c
        ${FFT_SOURCE_FILES}
        ${FFT_HEADER_FILES}
        ${UTIL_SOURCE_FILES}
        ${UTIL_HEADER_FILES})

add_executable(matmul
        tools/matmul/main.c
        ${MATRIX_SOURCE_FILES}
        ${MATRIX_HEADER_FILES}
        ${UTIL_SOURCE_FILES}
        ${UTIL_HEADER_FILES})
