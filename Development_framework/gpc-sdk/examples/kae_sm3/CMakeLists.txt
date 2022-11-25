cmake_minimum_required(VERSION 2.8)
project(sm3_demo C)

set(SRCS
    src/sm3_demo.c
    src/sm3.c
    src/wd_sm3.c
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

add_executable(sm3_demo ${SRCS})

target_link_libraries(sm3_demo wd)
