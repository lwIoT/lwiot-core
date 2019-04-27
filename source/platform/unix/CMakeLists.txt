#
# Unix CMake build file.
#
# Author: Michel Megens
# Email:  dev@bietje.net

include(${PROJECT_SOURCE_DIR}/cmake/mbedtls.cmake)
find_package(Threads REQUIRED)

SET(UNIX_DIR ${PROJECT_SOURCE_DIR}/source/platform/unix)
SET(HOSTED_DIR ${PROJECT_SOURCE_DIR}/source/platform/hosted)
SET(UNIX_SOURCE_FILES ${UNIX_DIR}/unix.c ${HOSTED_DIR}/timer.c ${HOSTED_DIR}/hostedgpiochip.cpp)

SET(PORT_SOURCE_FILES
	soc.c
	unix.c
	${PROJECT_SOURCE_DIR}/external/lwiot-mbedtls/mbedtls.c
	${UNIX_SOURCE_FILES}
)

add_library(lwiot-platform ${PORT_SOURCE_FILES})
include_directories(lwiot-platform PUBLIC ${PROJECT_SOURCE_DIR}/source/platform/unix ${HOSTED_DIR}/include)
target_link_libraries(lwiot-platform ${MBEDTLS_LIBRARY} ${MBEDX509_LIBRARY} ${MBEDCRYPTO_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

