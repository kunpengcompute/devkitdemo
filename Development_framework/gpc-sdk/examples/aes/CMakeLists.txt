cmake_minimum_required(VERSION 2.8)
project(aes C ASM)

set(SRCS
    src/aesv8-armx.S
    src/openssl.c
    src/aes.c
)

include_directories(
    include
)

# Default build type is debug
set(CMAKE_BUILD_TYPE "Debug")

# Set debug compilation option
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_compile_options(-g)
endif()

add_executable(aes ${SRCS})

target_link_libraries(aes)
