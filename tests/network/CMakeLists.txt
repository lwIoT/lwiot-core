include( ${PROJECT_SOURCE_DIR}/cmake/port.cmake )

include_directories(
	${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR} ${EXTRA_INCLUDE_DIRECTORIES}
	${LWIOT_PORT_INCLUDE_DIRECTORIES} ${PROJECT_SOURCE_DIR}/source/lib/json/include
)

if(HAVE_RTOS)
	link_directories(${FREERTOS_LIB_DIR})
endif()

add_executable(tcp-test tcp_test.cpp)
target_link_libraries(tcp-test lwiot ${LWIOT_SYSTEM_LIBS})
