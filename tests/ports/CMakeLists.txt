include( ${PROJECT_SOURCE_DIR}/cmake/port.cmake )

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR} ${PORT_INCLUDE_DIR})
if(HAVE_RTOS)
	link_directories(${FREERTOS_LIB_DIR})
endif()

if(WIN32)
	SET (PLATFORM lwiot-platform)
else()
	SET (PLATFORM -Wl,--whole-archive lwiot-platform -Wl,--no-whole-archive)
endif()

add_executable(init-test init_test.cpp)
target_link_libraries(init-test lwiot ${PLATFORM} lwiot ${LWIOT_SYSTEM_LIBS})

add_executable(timer-test timer_test.cpp)
target_link_libraries(timer-test lwiot ${PLATFORM} lwiot ${LWIOT_SYSTEM_LIBS})

add_executable(thread-test thread_test.cpp)
target_link_libraries(thread-test lwiot ${PLATFORM} lwiot  ${LWIOT_SYSTEM_LIBS})

add_executable(events-test events_test.cpp)
target_link_libraries(events-test lwiot ${PLATFORM} lwiot ${LWIOT_SYSTEM_LIBS})

add_executable(dispatchqueue-policy-test dispatchqueue-policy_test.cpp)
target_link_libraries(dispatchqueue-policy-test lwiot ${PLATFORM} lwiot ${LWIOT_SYSTEM_LIBS})
