
SET(LWIOT_PORT_DIR ports/unix)
SET(LWIOT_PORT_SRCS ${LWIOT_PORT_DIR}/unix.c
	soc/unix.c
	io/gpio/hostedgpiochip.cpp
)

SET(LWIOT_PORT_HEADERS
	${LWIOT_PORT_DIR}/lwiot_arch.h
)

SET(PORT_INCLUDE_DIR
	${PYTHON_INCLUDE_DIRS}
	${PYTHON_NUMPY_INCLUDE_DIR}
	${PROJECT_SOURCE_DIR}/external/matplotlib-cpp
	${PROJECT_SOURCE_DIR}/source/platform/ports/unix
	${PROJECT_SOURCE_DIR}/source/platform/hosted/include
)

SET(PLATFORM_DIRECTORY ${PROJECT_SOURCE_DIR}/source/platform/unix)

find_package(Threads REQUIRED)
SET(LWIOT_SYSTEM_LIBS ${CMAKE_THREAD_LIBS_INIT} ${LWIOT_SYSTEM_LIBS})
link_directories(${PROJECT_SOURCE_DIR}/external/freertos/lib)

SET(UNIX True)
SET(LWIOT_SYSTEM_LIBS ${LWIOT_SYSTEM_LIBS} ${CMAKE_THREAD_LIBS_INIT})

SET(HAVE_JSON True CACHE BOOL "Build JSON library")
SET(HAVE_NETWORKING True)
