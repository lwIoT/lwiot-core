include( ${PROJECT_SOURCE_DIR}/cmake/port.cmake )

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR} ${LWIOT_PORT_INCLUDE_DIRECTORIES})
add_executable(string-test string_test.cpp)
target_link_libraries(string-test lwiot ${PCAP_LIBRARY} ${ESTACK_SYSTEM_LIBS})

add_executable(log-test log_test.cpp)
target_link_libraries(log-test lwiot ${PCAP_LIBRARY} ${ESTACK_SYSTEM_LIBS})
