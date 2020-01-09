include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}
	${PROJECT_SOURCE_DIR}/source/platform/hosted/include)

if(HAVE_RTOS)
	link_directories(${FREERTOS_LIB_DIR})
endif()

if(${WIN32})
	SET (PLATFORM lwiot-platform lwiot)
else()
	SET (PLATFORM -Wl,--whole-archive lwiot-platform -Wl,--no-whole-archive lwiot)
endif()

add_executable(string-test string_test.cpp)
target_link_libraries(string-test ${PLATFORM} ${LWIOT_SYSTEM_LIBS})

add_executable(log-test log_test.cpp)
target_link_libraries(log-test ${PLATFORM} ${LWIOT_SYSTEM_LIBS})

add_executable(vector-test vector_test.cpp)
target_link_libraries(vector-test ${PLATFORM} ${LWIOT_SYSTEM_LIBS})

add_executable(bufferedstream-test bufferedstream_test.cpp)
target_link_libraries(bufferedstream-test ${PLATFORM} ${LWIOT_SYSTEM_LIBS})

add_executable(bytebuffer-test bytebuffer_test.cpp)
target_link_libraries(bytebuffer-test ${PLATFORM} ${LWIOT_SYSTEM_LIBS})

add_executable(dac-test dac_test.cpp)
target_link_libraries(dac-test ${PLATFORM} ${LWIOT_SYSTEM_LIBS})

add_executable(hostedgpio-test hostedgpio_test.cpp)
target_link_libraries(hostedgpio-test ${PLATFORM} ${LWIOT_SYSTEM_LIBS} ${PYTHON_LIBRARIES})

add_executable(json-test json_test.cpp)
target_link_libraries(json-test ${PLATFORM} ${LWIOT_SYSTEM_LIBS})

add_executable(ringbuffer-test ringbuffer_test.cpp)
target_link_libraries(ringbuffer-test ${PLATFORM} ${LWIOT_SYSTEM_LIBS})

add_executable(ipaddress-test ipaddress_test.cpp)
target_link_libraries(ipaddress-test ${PLATFORM} ${LWIOT_SYSTEM_LIBS})

add_executable(datetime-test datetime_test.cpp)
target_link_libraries(datetime-test ${PLATFORM} ${LWIOT_SYSTEM_LIBS})

add_executable(spi-test spi_test.cpp)
target_link_libraries(spi-test ${PLATFORM} ${LWIOT_SYSTEM_LIBS} ${PYTHON_LIBRARIES})

add_executable(i2cmessage-test i2cmessage_test.cpp)
target_link_libraries(i2cmessage-test ${PLATFORM} ${LWIOT_SYSTEM_LIBS} ${PYTHON_LIBRARIES})

add_executable(function-test function_test.cpp)
target_link_libraries(function-test ${PLATFORM} ${LWIOT_SYSTEM_LIBS} ${PYTHON_LIBRARIES})

add_executable(typetraits-test typetraits_test.cpp)
target_link_libraries(typetraits-test ${PLATFORM} ${LWIOT_SYSTEM_LIBS} ${PYTHON_LIBRARIES})

add_executable(leddriver-test leddriver_test.cpp)
target_link_libraries(leddriver-test ${PLATFORM} ${LWIOT_SYSTEM_LIBS} ${PYTHON_LIBRARIES})

add_executable(linkedlist_test linkedlist_test.cpp)
target_link_libraries(linkedlist_test ${PLATFORM} ${LWIOT_SYSTEM_LIBS} ${PYTHON_LIBRARIES})

add_executable(skiplist_test skiplist_test.cpp)
target_link_libraries(skiplist_test ${PLATFORM} ${LWIOT_SYSTEM_LIBS} ${PYTHON_LIBRARIES})

add_executable(guid_test guid_test.cpp)
target_link_libraries(guid_test ${PLATFORM} ${LWIOT_SYSTEM_LIBS} ${PYTHON_LIBRARIES})

add_executable(map_test map_test.cpp)
target_link_libraries(map_test ${PLATFORM} ${LWIOT_SYSTEM_LIBS} ${PYTHON_LIBRARIES})

add_executable(sharedpointer_test sharedpointer_test.cpp)
target_link_libraries(sharedpointer_test ${PLATFORM} ${LWIOT_SYSTEM_LIBS} ${PYTHON_LIBRARIES})

add_executable(tuple_test tuple_test.cpp)
target_link_libraries(tuple_test ${PLATFORM} ${LWIOT_SYSTEM_LIBS} ${PYTHON_LIBRARIES})

add_executable(fileio_test fileio_test.cpp)
target_link_libraries(fileio_test ${PLATFORM} ${LWIOT_SYSTEM_LIBS} ${PYTHON_LIBRARIES})

add_executable(hashmap-test hashmap_test.cpp)
target_link_libraries(hashmap-test lwiot ${PLATFORM} lwiot ${LWIOT_SYSTEM_LIBS} ${PYTHON_LIBRARIES})
