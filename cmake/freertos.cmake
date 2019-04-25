SET(LWIOT_PORT_DIR ${PROJECT_SOURCE_DIR}/source/ports/freertos)
SET(LWIOT_PORT_SRCS
    ${LWIOT_PORT_DIR}/rtos.c
)

SET(LWIOT_PORT_HEADERS
${LWIOT_PORT_DIR}/lwiot_arch.h)

SET(FREERTOS_DIR "${PROJECT_SOURCE_DIR}/external/freertos" CACHE STRING "FreeRTOS directory")
SET(FREERTOS_CONFIG_DIR "${PROJECT_SOURCE_DIR}/external/freertos/config" CACHE STRING "FreeRTOS config directory")
SET(FREERTOS_LIB_DIR "${FREERTOS_DIR}/lib")

SET(HAVE_RTOS True CACHE BOOL "Build as RTOS")

SET(PORT_INCLUDE_DIR
	${FREERTOS_CONFIG_DIR}
	${FREERTOS_DIR}/Source/include
	${FREERTOS_DIR}/Source/portable/${COMPILER}/${PORT}
	${PROJECT_SOURCE_DIR}/source/platform/hosted/include
)



if(UNIX)
	SET(PLATFORM_DIRECTORY ${PROJECT_SOURCE_DIR}/source/platform/unix-rtos)
	SET(LWIOT_SYSTEM_LIBS freertos pthread)
	SET(LWIOT_PORT_SRCS ${LWIOT_PORT_SRCS} ${LWIOT_PORT_DIR}/unix.c)
	find_package(Threads REQUIRED)
	SET(LWIOT_SYSTEM_LIBS ${CMAKE_THREAD_LIBS_INIT} ${LWIOT_SYSTEM_LIBS})
    link_directories(${PROJECT_SOURCE_DIR}/external/freertos/lib)
endif()
