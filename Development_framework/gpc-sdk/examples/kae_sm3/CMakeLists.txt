cmake_minimum_required(VERSION 2.8)
project(sm3_demo C)

set(SRCS
    src/sm3_demo.c
    src/sm3.c
    src/wd_sm3.c
)

include_directories(
    include
)

add_executable(sm3_demo ${SRCS})

target_link_libraries(sm3_demo wd)
