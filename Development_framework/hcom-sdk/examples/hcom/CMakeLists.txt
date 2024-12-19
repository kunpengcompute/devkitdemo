cmake_minimum_required(VERSION 2.8.12)
project(hcom_client CXX C)

execute_process(COMMAND cat /etc/os-release OUTPUT_VARIABLE OS_INFO OUTPUT_STRIP_TRAILING_WHITESPACE)

if(OS_INFO MATCHES "PRETTY_NAME=\"CentOS Linux 7 \\(AltArch\\)\"")
    execute_process(COMMAND cat /etc/redhat-release OUTPUT_VARIABLE CENTOS_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
    if(CENTOS_VERSION MATCHES "7.6.1810")
	    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    endif()
endif()

set(CMAKE_BUILD_TYPE Debug)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
include_directories("/usr/include/kunpeng-hcom-devel")
LINK_DIRECTORIES("/usr/local/lib/kunpeng-hcom")
file(GLOB_RECURSE SOURCES "*.cpp")

foreach (SOURCE ${SOURCES})
    get_filename_component(EXECUTABLE ${SOURCE} NAME_WE)
    add_executable(${EXECUTABLE} ${SOURCE})
    target_link_libraries(${EXECUTABLE} hcom pthread rt)
endforeach ()
