include( ${PROJECT_SOURCE_DIR}/cmake/port.cmake )

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR} ${LWIOT_PORT_INCLUDE_DIRECTORIES})

if(HAVE_RTOS)
	link_directories(${FREERTOS_LIB_DIR})
endif()

add_executable(string-test string_test.cpp)
target_link_libraries(string-test lwiot ${PCAP_LIBRARY} ${LWIOT_SYSTEM_LIBS})

add_executable(log-test log_test.cpp)
target_link_libraries(log-test lwiot ${PCAP_LIBRARY} ${LWIOT_SYSTEM_LIBS})

add_executable(vector-test vector_test.cpp)
target_link_libraries(vector-test lwiot ${PCAP_LIBRARY} ${LWIOT_SYSTEM_LIBS})

add_executable(bufferedstream-test bufferedstream_test.cpp)
target_link_libraries(bufferedstream-test lwiot ${LWIOT_SYSTEM_LIBS})

add_executable(bytebuffer-test bytebuffer_test.cpp)
target_link_libraries(bytebuffer-test lwiot ${LWIOT_SYSTEM_LIBS})

add_executable(dac-test dac_test.cpp)
target_link_libraries(dac-test lwiot ${LWIOT_SYSTEM_LIBS})

add_executable(matplotlib-test matplotlib_test.cpp)
target_link_libraries(matplotlib-test lwiot ${LWIOT_SYSTEM_LIBS} ${PYTHON_LIBRARIES})
