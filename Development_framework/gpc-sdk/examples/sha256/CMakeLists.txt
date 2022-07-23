cmake_minimum_required(VERSION 2.8)
enable_language(ASM)
project(sha256_demo C)

# ADD source files
set(SRC src/main.c)
set(SHA256_C src/sha256.c)
set(SHA256_ASM src/sha256_armv8.S)

# Set target file
set(TARGET_FILE sha256)

# Add header file include directories
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

# ADD compiler option
add_definitions("-march=armv8-a+crypto")

add_executable(${TARGET_FILE} ${SRC} ${SHA256_C} ${SHA256_ASM})
