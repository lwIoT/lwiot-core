#
# Source CMakeLists.txt
#

include( ${PROJECT_SOURCE_DIR}/cmake/port.cmake )


if(FREERTOS)
    SET(CMAKE_REQUIRED_INCLUDES ${LWIOT_PORT_INCLUDE_DIRECTORIES})
    SET(CMAKE_EXTRA_INCLUDE_FILES FreeRTOS.h task.h queue.h semphr.h timers.h)
    CHECK_TYPE_SIZE(TaskHandle_t HAVE_TASKHANDLE_T)
    CHECK_TYPE_SIZE(QueueHandle_t HAVE_QUEUEHANDLE_T)
    CHECK_TYPE_SIZE(SemaphoreHandle_t HAVE_SEMAPHOREHANDLE_T)
    CHECK_TYPE_SIZE(TimerHandle_t HAVE_TIMERHANDLE_T)
    CHECK_TYPE_SIZE(TickType_t HAVE_TICKTYPE_T)
    CHECK_INCLUDE_FILES (stdbool.h  HAVE_STDBOOL_H)
endif()

set(SOURCES
    init.c
    string.cpp
    ${LWIOT_PORT_SRCS}
)

set(GENERIC_HEADERS
    lwiot.h
    list.h
    error.h
    compiler.h
    compiler-gcc.h
    compiler-vc.h
    config.h.in
    port.h
    types.h
    string.h
    thread.h
)

# Include timer implementation on UNIX and Windows builds
IF(NOT CMAKE_SYSTEM_NAME MATCHES Generic)
SET(SOURCES
${SOURCES}
ports/timer.c)
ENDIF()

set(BASE_HDRS
    ${PROJECT_SOURCE_DIR}/include/lwiot.h
    ${PROJECT_BINARY_DIR}/config.h
)

SET(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

FUNCTION( prepend_path SOURCE_FILES INC_PATH )
    FOREACH( SOURCE_FILE ${${SOURCE_FILES}} )
        SET( MODIFIED ${MODIFIED} ${INC_PATH}/${SOURCE_FILE} )
    ENDFOREACH()
    SET( ${SOURCE_FILES} ${MODIFIED} PARENT_SCOPE )
ENDFUNCTION()

prepend_path(GENERIC_HEADERS ${INCLUDE_DIR}/lwiot)

include_directories(${PROJECT_SOURCE_DIR}/include ${CMAKE_BINARY_DIR} ${EXTRA_INCLUDE_DIRECTORIES} ${LWIOT_PORT_INCLUDE_DIRECTORIES})

if(EXTRA_LINK_DIRECTORIES)
    link_directories(${EXTRA_LINK_DIRECTORIES})
endif()

add_library(lwiot ${SOURCES} ${GENERIC_HEADERS} ${BASE_HDRS})

CONFIGURE_FILE(${INCLUDE_DIR}/lwiot/config.h.in ${CMAKE_BINARY_DIR}/config.h)
