include( ${PROJECT_SOURCE_DIR}/cmake/port.cmake )

include_directories(
    ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR} ${EXTRA_INCLUDE_DIRECTORIES}
    ${LWIOT_PORT_INCLUDE_DIRECTORIES} ${PROJECT_SOURCE_DIR}/source/lib/json/include
)

if(HAVE_RTOS)
	link_directories(${FREERTOS_LIB_DIR})
endif()

add_executable(string-test string_test.cpp)
target_link_libraries(string-test lwiot ${LWIOT_SYSTEM_LIBS})

add_executable(log-test log_test.cpp)
target_link_libraries(log-test lwiot ${LWIOT_SYSTEM_LIBS})

add_executable(vector-test vector_test.cpp)
target_link_libraries(vector-test lwiot ${LWIOT_SYSTEM_LIBS})

add_executable(bufferedstream-test bufferedstream_test.cpp)
target_link_libraries(bufferedstream-test lwiot ${LWIOT_SYSTEM_LIBS})

add_executable(bytebuffer-test bytebuffer_test.cpp)
target_link_libraries(bytebuffer-test lwiot ${LWIOT_SYSTEM_LIBS})

add_executable(dac-test dac_test.cpp)
target_link_libraries(dac-test lwiot ${LWIOT_SYSTEM_LIBS})

add_executable(matplotlib-test matplotlib_test.cpp)
target_link_libraries(matplotlib-test lwiot ${LWIOT_SYSTEM_LIBS} ${PYTHON_LIBRARIES})

add_executable(hostedgpio-test hostedgpio_test.cpp)
target_link_libraries(hostedgpio-test lwiot ${LWIOT_SYSTEM_LIBS} ${PYTHON_LIBRARIES})

add_executable(json-test json_test.cpp)
target_link_libraries(json-test lwiot ${LWIOT_SYSTEM_LIBS})

add_executable(ipaddress-test ipaddress_test.cpp)
target_link_libraries(ipaddress-test lwiot ${LWIOT_SYSTEM_LIBS})

add_executable(datetime-test datetime_test.cpp)
target_link_libraries(datetime-test lwiot ${LWIOT_SYSTEM_LIBS})

add_executable(spi-test spi_test.cpp)
target_link_libraries(spi-test lwiot ${LWIOT_SYSTEM_LIBS} ${PYTHON_LIBRARIES})

add_executable(i2cmessage-test i2cmessage_test.cpp)
target_link_libraries(i2cmessage-test lwiot ${LWIOT_SYSTEM_LIBS} ${PYTHON_LIBRARIES})
