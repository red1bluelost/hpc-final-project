cmake_minimum_required(VERSION 3.22)
project(fp_cpp)

set(CMAKE_CXX_STANDARD 20)

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

set(MATRIX_SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/Matrix/Matrix.cpp
        ${PROJECT_SOURCE_DIR}/Matrix/Multiply.cpp)
set(MATRIX_HEADER_FILES
        ${PROJECT_HEADER_DIR}/Matrix/Matrix.h
        ${PROJECT_HEADER_DIR}/Matrix/Multiply.h)

set(UTIL_SOURCE_FILES)
set(UTIL_HEADER_FILES
        ${PROJECT_HEADER_DIR}/Util/Math.h
        ${PROJECT_HEADER_DIR}/Util/ArgParse.h)

set(SOURCE_FILES
        ${MATRIX_SOURCE_FILES}
        ${UTIL_SOURCE_FILES})
set(HEADER_FILES
        ${MATRIX_HEADER_FILES}
        ${UTIL_HEADER_FILES})

add_executable(matmul
        tools/matmul/main.cpp
        ${SOURCE_FILES}
        ${HEADER_FILES})
