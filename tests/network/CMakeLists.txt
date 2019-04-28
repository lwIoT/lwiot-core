include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}
	${PROJECT_SOURCE_DIR}/source/platform/hosted/include)

if(HAVE_RTOS)
	link_directories(${FREERTOS_LIB_DIR})
endif()

IF(WIN32)
SET (PLATFORM lwiot-platform lwiot)
ELSE()
SET (PLATFORM -Wl,--whole-archive lwiot-platform -Wl,--no-whole-archive lwiot)
ENDIF()

add_executable(tcp-client_test tcp-client_test.cpp)
target_link_libraries(tcp-client_test lwiot ${PLATFORM} ${LWIOT_SYSTEM_LIBS})

add_executable(tcp-server_test tcp-server_test.cpp)
target_link_libraries(tcp-server_test lwiot ${PLATFORM} ${LWIOT_SYSTEM_LIBS})

add_executable(http-server_test http-server_test.cpp)
target_link_libraries(http-server_test lwiot ${PLATFORM} ${LWIOT_SYSTEM_LIBS})

add_executable(udp-server_test udp-server_test.cpp)
target_link_libraries(udp-server_test lwiot ${PLATFORM} ${LWIOT_SYSTEM_LIBS})

add_executable(udp-client_test udp-client_test.cpp)
target_link_libraries(udp-client_test lwiot ${PLATFORM} ${LWIOT_SYSTEM_LIBS})

add_executable(captiveportal_test captiveportal_test.cpp)
target_link_libraries(captiveportal_test lwiot ${PLATFORM} ${LWIOT_SYSTEM_LIBS})

IF(UNIX)
add_executable(sslclient_test sslclient_test.cpp)
target_link_libraries(sslclient_test lwiot ${PLATFORM} ${LWIOT_SYSTEM_LIBS})
ENDIF()
