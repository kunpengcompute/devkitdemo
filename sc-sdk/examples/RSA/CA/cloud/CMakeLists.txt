cmake_minimum_required (VERSION 2.8)
project (rsa_demo)
cmake_policy (SET CMP0037 OLD)

# Add source files
set(SRCS
    ../rsa_ca_demo.c
)

# Add header file include directories
include_directories(
    /usr/include/itrustee_sdk/CA
)

# Set options
set(CMAKE_RSA_FLAGS "-fstack-protector-strong -fPIC")
set(CMAKE_RSA_FLAGS "${CMAKE_RSA_FLAGS} -z text -z now -z relro -z noexecstack -pie")

# Set output dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

# Generate execute file
add_executable(rsa_demo ${SRCS})
target_link_libraries(rsa_demo teec_adaptor dl pthread)
set_target_properties(rsa_demo PROPERTIES COMPILE_FLAGS ${CMAKE_RSA_FLAGS})
 
# Clean output
add_custom_target(clean
    COMMAND rm -rf ${PROJECT_SOURCE_DIR}/rsa_demo
    COMMAND @echo "clean up"
)
