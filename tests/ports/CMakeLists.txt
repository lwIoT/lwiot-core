if(HAVE_RTOS)
include(${PROJECT_SOURCE_DIR}/cmake/freertos.cmake)
endif()

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}
	${PROJECT_SOURCE_DIR}/source/platform/hosted/include
	${PORT_INCLUDE_DIR})
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

add_executable(fsm-policy-test fsm-policy_test.cpp)
target_link_libraries(fsm-policy-test lwiot ${PLATFORM} lwiot ${LWIOT_SYSTEM_LIBS})

add_executable(async-fsm-test async-fsm_test.cpp)
target_link_libraries(async-fsm-test lwiot ${PLATFORM} lwiot ${LWIOT_SYSTEM_LIBS})

add_executable(fsm-test fsm_test.cpp)
target_link_libraries(fsm-test lwiot ${PLATFORM} lwiot ${LWIOT_SYSTEM_LIBS})

add_executable(dispatchqueue-test dpq_test.cpp)
target_link_libraries(dispatchqueue-test lwiot ${PLATFORM} lwiot ${LWIOT_SYSTEM_LIBS})

add_executable(eventqueue-test evq_test.cpp)
target_link_libraries(eventqueue-test lwiot ${PLATFORM} lwiot ${LWIOT_SYSTEM_LIBS})

add_executable(dispatchqueue-policy-test dpq-policy_test.cpp)
target_link_libraries(dispatchqueue-policy-test lwiot ${PLATFORM} lwiot ${LWIOT_SYSTEM_LIBS})

add_executable(hardwarei2c-test hardwarei2c_test.cpp)
target_link_libraries(hardwarei2c-test lwiot ${PLATFORM} lwiot ${LWIOT_SYSTEM_LIBS})
