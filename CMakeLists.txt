cmake_minimum_required(VERSION 3.0)
project(lwIoT)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)

add_subdirectory(source)

if( NOT CMAKE_SYSTEM_NAME MATCHES Generic )
    add_subdirectory(scripts)
endif()