#
# Unix RTOS CMake build file.
#
# Author: Michel Megens
# Email:  dev@bietje.net
#

find_package(Threads REQUIRED)

include (${PROJECT_SOURCE_DIR}/cmake/freertos.cmake)

SET(HOSTED_DIR ${PROJECT_SOURCE_DIR}/source/platform/hosted)
SET(HOSTED_DIR ${PROJECT_SOURCE_DIR}/source/platform/hosted)

SET(UNIX_SOURCE_FILES
	${HOSTED_DIR}/hostedgpiochip.cpp
	${HOSTED_DIR}/i2chal.c
	${HOSTED_DIR}/i2cdbg.c
)

SET(PORT_SOURCE_FILES
	soc.c
	unix.c
	${PROJECT_SOURCE_DIR}/external/lwiot-freertos/rtos.c
	${UNIX_SOURCE_FILES}

)

add_library(lwiot-platform ${PORT_SOURCE_FILES})
include_directories(lwiot-platform PUBLIC ${PROJECT_SOURCE_DIR}/source/platform/unix ${HOSTED_DIR}/include ${PORT_INCLUDE_DIR})
target_link_libraries(lwiot-platform mbedtls mbedx509 mbedcrypto ${CMAKE_THREAD_LIBS_INIT})
