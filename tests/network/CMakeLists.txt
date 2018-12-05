include( ${PROJECT_SOURCE_DIR}/cmake/port.cmake )

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR} ${PORT_INCLUDE_DIR})
if(HAVE_RTOS)
	link_directories(${FREERTOS_LIB_DIR})
endif()
SET (PLATFORM -Wl,--whole-archive lwiot-platform -Wl,--no-whole-archive lwiot)

add_executable(tcp-client_test tcp-client_test.cpp)
target_link_libraries(tcp-client_test lwiot ${PLATFORM} ${LWIOT_SYSTEM_LIBS})

#add_executable(tcp-server_test tcp-server_test.cpp)
#target_link_libraries(tcp-server_test lwiot ${LWIOT_SYSTEM_LIBS})

#add_executable(http-server_test http-server_test.cpp)
#target_link_libraries(http-server_test lwiot ${LWIOT_SYSTEM_LIBS})
