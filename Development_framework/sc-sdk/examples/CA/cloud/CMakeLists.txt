cmake_minimum_required (VERSION 2.8)
project (rsa_demo)

# set kunpeng-sc-devel path
set(iTrustee_SDK_PATH /usr/local/kunpeng-sc-devel)

# Add source files
set(SRCS
    ../rsa_ca_demo.c
    ${iTrustee_SDK_PATH}/source/itrustee_sdk/src/CA/libteec_adaptor.c
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
target_link_libraries(rsa_demo dl pthread)
set_target_properties(rsa_demo PROPERTIES COMPILE_FLAGS ${CMAKE_RSA_FLAGS})
 
# Clean output
add_custom_target(clean
    COMMAND rm -rf ${PROJECT_SOURCE_DIR}/ca_demo
    COMMAND @echo "clean up"
)
