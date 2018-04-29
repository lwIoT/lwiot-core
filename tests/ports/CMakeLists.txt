include( ${PROJECT_SOURCE_DIR}/cmake/port.cmake )

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR} ${LWIOT_PORT_INCLUDE_DIRECTORIES})
add_executable(init-test init_test.cpp)
target_link_libraries(init-test lwiot ${PCAP_LIBRARY} ${ESTACK_SYSTEM_LIBS})
