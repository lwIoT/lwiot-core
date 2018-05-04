include( ${PROJECT_SOURCE_DIR}/cmake/port.cmake )

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR} ${LWIOT_PORT_INCLUDE_DIRECTORIES})
if(HAVE_RTOS)
	link_directories(${FREERTOS_LIB_DIR})
endif()
add_executable(init-test init_test.cpp)
target_link_libraries(init-test lwiot ${PCAP_LIBRARY} ${LWIOT_SYSTEM_LIBS})

add_executable(timer-test timer_test.cpp)
target_link_libraries(timer-test lwiot ${PCAP_LIBRARY} ${LWIOT_SYSTEM_LIBS})

add_executable(thread-test thread_test.cpp)
target_link_libraries(thread-test lwiot ${PCAP_LIBRARY} ${LWIOT_SYSTEM_LIBS})

add_executable(events-test events_test.cpp)
target_link_libraries(events-test lwiot ${PCAP_LIBRARY} ${LWIOT_SYSTEM_LIBS})
