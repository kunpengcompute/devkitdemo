cmake_minimum_required(VERSION 2.8)
project(sm4_demo C)

set(SRCS
    src/sm4_demo.c
    src/e_sm4.c
    src/sm4.c
    src/wd_sm4.c
)

include_directories(
    include
)

add_executable(sm4_demo ${SRCS})

target_link_libraries(sm4_demo wd)
