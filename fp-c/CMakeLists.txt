cmake_minimum_required(VERSION 3.18)
project(fp_c C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_USE_PTHREADS_INIT true)
set(CMAKE_USE_PTHREADS_INIT ON)

find_package(Threads REQUIRED)

include(CheckCCompilerFlag)
CHECK_C_COMPILER_FLAG("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
if (COMPILER_SUPPORTS_MARCH_NATIVE)
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
endif ()

add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        $<$<CONFIG:DEBUG>:-fsanitize=address>
        $<$<CONFIG:DEBUG>:-g3>
        $<$<CONFIG:DEBUG>:-Og>
        $<$<CONFIG:RELEASE>:-g0>
        $<$<CONFIG:RELEASE>:-O3>)

add_link_options(
        $<$<CONFIG:DEBUG>:-fsanitize=address>)


set(PROJECT_SOURCE_DIR lib)
set(PROJECT_HEADER_DIR include)

include_directories(${PROJECT_HEADER_DIR})

set(FFT_SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/fft/fft.c
        ${PROJECT_SOURCE_DIR}/fft/vector.c)
set(FFT_HEADER_FILES
        ${PROJECT_HEADER_DIR}/fft/fft.h
        ${PROJECT_HEADER_DIR}/fft/vector.h)

set(MATRIX_SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/matrix/matrix.c
        ${PROJECT_SOURCE_DIR}/matrix/multiply.c)
set(MATRIX_HEADER_FILES
        ${PROJECT_HEADER_DIR}/matrix/matrix.h
        ${PROJECT_HEADER_DIR}/matrix/multiply.h)

set(UTIL_SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/util/argparse.c
        ${PROJECT_SOURCE_DIR}/util/math.c)
set(UTIL_HEADER_FILES
        ${PROJECT_HEADER_DIR}/util/argparse.h
        ${PROJECT_HEADER_DIR}/util/math.h)

add_executable(fft
        tools/fft/main.c
        ${FFT_SOURCE_FILES}
        ${FFT_HEADER_FILES}
        ${UTIL_SOURCE_FILES}
        ${UTIL_HEADER_FILES})
target_link_libraries(fft m)

add_executable(matmul
        tools/matmul/main.c
        ${MATRIX_SOURCE_FILES}
        ${MATRIX_HEADER_FILES}
        ${UTIL_SOURCE_FILES}
        ${UTIL_HEADER_FILES})
target_link_libraries(matmul Threads::Threads)
