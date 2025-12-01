cmake_minimum_required(VERSION 3.18.0)
project(MyProgram)
enable_language(Fortran)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -fallow-argument-mismatch")

file(GLOB SRCS ${PROJECT_SOURCE_DIR}/*)
add_executable(myprogram ${SRCS})

