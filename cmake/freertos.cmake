SET(LWIOT_PORT_DIR ${PROJECT_SOURCE_DIR}/source/ports/freertos)
SET(LWIOT_PORT_SRCS
    ${LWIOT_PORT_DIR}/rtos.c
	ports/thread.cpp
)

SET(LWIOT_PORT_HEADERS
${LWIOT_PORT_DIR}/arch.h)

if(FREERTOS_CONFIG)
    include(${FREERTOS_CONFIG})
else()
    SET(FREERTOS_DIR "${PROJECT_SOURCE_DIR}/external/freertos" CACHE STRING "FreeRTOS directory")

    SET(FREERTOS_CONFIG_DIR "${PROJECT_SOURCE_DIR}/external/freertos/config" CACHE STRING "FreeRTOS config directory")
    SET(FREERTOS_INCLUDE_EXTRA "" CACHE STRING "Extra include libs")


endif()

SET(HAVE_RTOS True)

if(FREERTOS_MANUAL)
    message(STATUS ${LWIOT_PORT_INCLUDE_DIR})
else()
    SET(LWIOT_PORT_INCLUDE_DIRECTORIES
            ${PROJECT_SOURCE_DIR}/source/ports/freertos
            ${FREERTOS_CONFIG_DIR}
            ${FREERTOS_DIR}/Source/include
            ${FREERTOS_DIR}/Source/portable/${COMPILER}/${PORT}
    )
endif()

SET(EXTRA_INCLUDE_DIRECTORIES "${EXTRA_INCLUDE_DIRECTORIES} ${FREERTOS_INCLUDE_EXTRA}")


if(${PORT} MATCHES "unix")
	SET(LWIOT_PORT_SRCS ${LWIOT_PORT_SRCS} ${LWIOT_PORT_DIR}/unix.c)
	find_package(Threads REQUIRED)
	SET(LWIOT_SYSTEM_LIBS ${CMAKE_THREAD_LIBS_INIT} ${LWIOT_SYSTEM_LIBS})
endif()
