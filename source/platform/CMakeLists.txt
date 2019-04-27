#
# CMakeLists file for the platform library.
#

if(WIN32)
add_subdirectory(win32)
elseif(UNIX AND FREERTOS)
add_subdirectory(unix-rtos)
elseif(UNIX)
add_subdirectory(unix)
endif()

SET(SOURCES ${LWIOT_PORT_SRCS})
INSTALL(FILES ${LWIOT_PORT_HEADERS} DESTINATION include)
