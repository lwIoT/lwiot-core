include( ${PROJECT_SOURCE_DIR}/cmake/port.cmake )

include_directories(
	${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR} ${EXTRA_INCLUDE_DIRECTORIES}
	${LWIOT_PORT_INCLUDE_DIRECTORIES} ${PROJECT_SOURCE_DIR}/source/lib/json/include
)

if(HAVE_RTOS)
	link_directories(${FREERTOS_LIB_DIR})
endif()

add_executable(tcp-client_test tcp-client_test.cpp)
target_link_libraries(tcp-client_test lwiot ${LWIOT_SYSTEM_LIBS})

add_executable(tcp-server_test tcp-server_test.cpp)
target_link_libraries(tcp-server_test lwiot ${LWIOT_SYSTEM_LIBS})

add_executable(http-server_test http-server_test.cpp)
target_link_libraries(http-server_test lwiot ${LWIOT_SYSTEM_LIBS})
