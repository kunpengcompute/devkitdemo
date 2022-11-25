cmake_minimum_required(VERSION 2.8)
project(aes C ASM)

set(SRCS
    src/aesv8-armx.S
    src/openssl.c
    src/wd.c
    src/aes.c
)

# Set KAELIB path
set(KAELIB_PATH /usr/local/lib)

# Set lib path
link_directories(${KAELIB_PATH})

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

target_link_libraries(aes wd)
