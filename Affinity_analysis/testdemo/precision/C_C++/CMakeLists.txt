cmake_minimum_required(VERSION 3.20.4)
project(C-FFT)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# 设置C编译器
set(CMAKE_C_COMPILER "gcc")
# 设置C++编译器
set(CMAKE_CXX_COMPILER "g++")

# set(CMAKE_C_COMPILER "clang")
# # 设置C++编译器
# set(CMAKE_CXX_COMPILER "clang++")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -floop-optimize")

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

file(GLOB_RECURSE LIB_SRC ./lib/*.c)

add_executable(test ${CMAKE_CURRENT_LIST_DIR}/test.c ${LIB_SRC})
target_link_libraries(test PUBLIC
-lm
# -lcruiser
# -lcrypto
)