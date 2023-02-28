cmake_minimum_required (VERSION 2.8)
project (rsa_demo)

# Set variable
# kunpeng-sc-devel path set
set(iTrustee_SDK_PATH /usr/local/kunpeng-sc-devel)
set(iTrustee_SDK_HEADER_PATH /usr/include/itrustee_sdk)
set(SIGNTOOL_DIR ${iTrustee_SDK_PATH}/utils/signtools)
set(LIBC ${iTrustee_SDK_HEADER_PATH}/thirdparty/musl)

# Add source files
file(GLOB SRCS "./*.c")
set(SRCS ${SRCS} ../rsa_ta_interface.c)

# Add header file include directories
include_directories(
    ${LIBC}/libc
    ${LIBC}/libc/arch/aarch64
    ${LIBC}/libc/arch/aarch64/bits
    ${LIBC}/libc/arch/generic
    ${LIBSECURE}/include
    ${iTrustee_SDK_HEADER_PATH}/TA
    ${iTrustee_SDK_HEADER_PATH}/TA/huawei_ext
    ${PROJECT_SOURCE_DIR}/include
)

# Set options
set(CMAKE_RSA_FLAGS "-DAPI_LEVEL=1 -O -W -Wall -Werror -fno-short-enums -fno-omit-frame-pointer -fstack-protector-strong")
set(CMAKE_RSA_FLAGS "${CMAKE_RSA_FLAGS} -Wextra -nostdinc -nodefaultlibs -march=armv8-a -Os -Wno-main -fPIC")
set(CMAKE_RSA_FLAGS "${CMAKE_RSA_FLAGS} -Wno-error=unused-parameter -Wno-error=unused-but-set-variable")
set(CMAKE_RSA_LD_FLAGS "-s -z text -z now -z relro -z noexecstack -shared -nodefaultlibs")
set(CMAKE_RSA_LD_FLAGS "${CMAKE_RSA_LD_FLAGS} -T${iTrustee_SDK_PATH}/utils/tools/ta_link_64.ld")

# Set output dir
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

# Generate .so file
add_library(tmp_object OBJECT ${SRCS})
set_target_properties(tmp_object PROPERTIES COMPILE_FLAGS ${CMAKE_RSA_FLAGS})
add_library(combine SHARED $<TARGET_OBJECTS:tmp_object>)
set_target_properties(combine PROPERTIES LINK_FLAGS ${CMAKE_RSA_LD_FLAGS})
 
add_custom_command(TARGET combine
    POST_BUILD
    COMMAND cp -f ${PROJECT_SOURCE_DIR}/../manifest.txt ${PROJECT_SOURCE_DIR}
    COMMAND python3 -B ${SIGNTOOL_DIR}/signtool_v3.py ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR} --privateCfg ${PROJECT_SOURCE_DIR}/config_cloud.ini
)

# Clean output
add_custom_target(clean
    COMMAND rm -rf ${PROJECT_SOURCE_DIR}/libcombine.so ${PROJECT_SOURCE_DIR}/*.sec
    COMMAND @echo "clean up"
)
