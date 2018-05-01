#
# Source CMakeLists.txt
#

include(CheckIncludeFiles)
include(CheckTypeSize)
include(CheckCSourceCompiles)

include( ${PROJECT_SOURCE_DIR}/cmake/port.cmake )

set(SOURCES
    init.c
    log.c
    string.cpp
	ports/thread.cpp
	ports/lock.cpp
	ports/timer.cpp
    ${LWIOT_PORT_SRCS}
)

set(GENERIC_HEADERS
    lwiot.h
    log.h
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
	lock.h
	timer.h
)

# Include timer implementation on UNIX and Windows builds
IF(NOT CMAKE_SYSTEM_NAME MATCHES Generic)
SET(SOURCES
${SOURCES}
ports/timer.c)
ENDIF()

set(BASE_HDRS
    ${PROJECT_SOURCE_DIR}/include/lwiot.h
    ${PROJECT_BINARY_DIR}/lwiot_opts.h
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

SET(${SOURCES} ${SOURCES})
add_library(lwiot ${SOURCES} ${GENERIC_HEADERS} ${BASE_HDRS} ${LWIOT_PORT_HEADERS})
link_directories(${PROJECT_SOURCE_DIR}/external/freertos/lib)

CONFIGURE_FILE(${INCLUDE_DIR}/lwiot/config.h.in ${CMAKE_BINARY_DIR}/lwiot_opts.h)


foreach(source IN LISTS ${SOURCES})
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "Source Files\\${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()
