cmake_minimum_required(VERSION 2.8)
project(sm4_demo C)

set(SRCS
    src/sm4_demo.c
    src/e_sm4.c
    src/sm4.c
    src/wd_sm4.c
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

add_executable(sm4_demo ${SRCS})

target_link_libraries(sm4_demo wd)
