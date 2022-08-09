cmake_minimum_required(VERSION 2.8)
project(aes C ASM)

set(SRCS
    src/aesv8-armx.S
    src/openssl.c
    src/wd.c
    src/aes.c
)

include_directories(
    include
)

add_executable(aes ${SRCS})

target_link_libraries(aes wd)
