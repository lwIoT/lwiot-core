cmake_minimum_required(VERSION 3.0.0)
project(esp32_test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

add_custom_target(mytest COMMAND make -C ${mytest_SOURCE_DIR}
                         CLION_EXE_DIR=${PROJECT_BINARY_DIR})
