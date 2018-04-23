#
# Source CMakeLists.txt
#

include( ${PROJECT_SOURCE_DIR}/cmake/port.cmake )

set(SOURCES
    init.c
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

include_directories(${PROJECT_SOURCE_DIR}/include ${CMAKE_BINARY_DIR} ${LWIOT_PORT_INCLUDE_DIR})
add_library(lwiot ${SOURCES} ${GENERIC_HEADERS} ${BASE_HEADERS})

CONFIGURE_FILE(${INCLUDE_DIR}/lwiot/config.h.in ${CMAKE_BINARY_DIR}/config.h)
