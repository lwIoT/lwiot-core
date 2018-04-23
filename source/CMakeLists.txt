#
# Source CMakeLists.txt
#

include( ${PROJECT_SOURCE_DIR}/cmake/port.cmake )

set(SOURCES
    init.c
)

set(GENERIC_HEADERS
    lwiot.h
)

set(BASE_HDRS
    ${PROJECT_SOURCE_DIR}/include/lwiot.h
)

SET(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

FUNCTION( prepend_path SOURCE_FILES INC_PATH )
    FOREACH( SOURCE_FILE ${${SOURCE_FILES}} )
        SET( MODIFIED ${MODIFIED} ${INC_PATH}/${SOURCE_FILE} )
    ENDFOREACH()
    SET( ${SOURCE_FILES} ${MODIFIED} PARENT_SCOPE )
ENDFUNCTION()

prepend_path(GENERIC_HEADERS ${INCLUDE_DIR}/lwiot)

include_directories(${PROJECT_SOURCE_DIR}/include ${CMAKE_BINARY_DIR})
add_library(lwiot ${SOURCES} ${GENERIC_HEADERS} ${BASE_HEADERS})
