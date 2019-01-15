include( ${PROJECT_SOURCE_DIR}/cmake/port.cmake )

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR} ${PORT_INCLUDE_DIR})
SET (PLATFORM -Wl,--whole-archive lwiot-platform -Wl,--no-whole-archive lwiot)

if(HAVE_RTOS)
	link_directories(${FREERTOS_LIB_DIR})
endif()

add_executable(string-test string_test.cpp)
target_link_libraries(string-test lwiot ${PLATFORM} ${LWIOT_SYSTEM_LIBS})

add_executable(log-test log_test.cpp)
target_link_libraries(log-test lwiot ${PLATFORM} ${LWIOT_SYSTEM_LIBS})

add_executable(vector-test vector_test.cpp)
target_link_libraries(vector-test lwiot ${PLATFORM} ${LWIOT_SYSTEM_LIBS})

add_executable(bufferedstream-test bufferedstream_test.cpp)
target_link_libraries(bufferedstream-test lwiot ${PLATFORM} ${LWIOT_SYSTEM_LIBS})

add_executable(bytebuffer-test bytebuffer_test.cpp)
target_link_libraries(bytebuffer-test lwiot ${PLATFORM} ${LWIOT_SYSTEM_LIBS})

add_executable(dac-test dac_test.cpp)
target_link_libraries(dac-test lwiot ${PLATFORM} ${LWIOT_SYSTEM_LIBS})

add_executable(hostedgpio-test hostedgpio_test.cpp)
target_link_libraries(hostedgpio-test lwiot ${PLATFORM} ${LWIOT_SYSTEM_LIBS} ${PYTHON_LIBRARIES})

add_executable(json-test json_test.cpp)
target_link_libraries(json-test lwiot ${PLATFORM} ${LWIOT_SYSTEM_LIBS})

add_executable(ipaddress-test ipaddress_test.cpp)
target_link_libraries(ipaddress-test lwiot ${PLATFORM} ${LWIOT_SYSTEM_LIBS})

add_executable(datetime-test datetime_test.cpp)
target_link_libraries(datetime-test lwiot ${PLATFORM} ${LWIOT_SYSTEM_LIBS})

add_executable(spi-test spi_test.cpp)
target_link_libraries(spi-test lwiot ${PLATFORM} ${LWIOT_SYSTEM_LIBS} ${PYTHON_LIBRARIES})

add_executable(i2cmessage-test i2cmessage_test.cpp)
target_link_libraries(i2cmessage-test lwiot ${PLATFORM} ${LWIOT_SYSTEM_LIBS} ${PYTHON_LIBRARIES})

add_executable(function-test function_test.cpp)
target_link_libraries(function-test lwiot ${PLATFORM} ${LWIOT_SYSTEM_LIBS} ${PYTHON_LIBRARIES})

add_executable(typetraits-test typetraits_test.cpp)
target_link_libraries(typetraits-test lwiot ${PLATFORM} ${LWIOT_SYSTEM_LIBS} ${PYTHON_LIBRARIES})

add_executable(linkedlist_test linkedlist_test.cpp)
target_link_libraries(linkedlist_test lwiot ${PLATFORM} ${LWIOT_SYSTEM_LIBS} ${PYTHON_LIBRARIES})

add_executable(guid_test guid_test.cpp)
target_link_libraries(guid_test lwiot ${PLATFORM} ${LWIOT_SYSTEM_LIBS} ${PYTHON_LIBRARIES})

add_executable(map_test map_test.cpp)
target_link_libraries(map_test lwiot ${PLATFORM} ${LWIOT_SYSTEM_LIBS} ${PYTHON_LIBRARIES})

add_executable(sharedpointer_test sharedpointer_test.cpp)
target_link_libraries(sharedpointer_test lwiot ${PLATFORM} ${LWIOT_SYSTEM_LIBS} ${PYTHON_LIBRARIES})

add_executable(tuple_test tuple_test.cpp)
target_link_libraries(tuple_test lwiot ${PLATFORM} ${LWIOT_SYSTEM_LIBS} ${PYTHON_LIBRARIES})
