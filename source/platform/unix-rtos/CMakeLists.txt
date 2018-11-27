#
# Unix RTOS CMake build file.
#
# Author: Michel Megens
# Email:  dev@bietje.net

include(${PROJECT_SOURCE_DIR}/source/platform/ports/freertos/freertos.cmake)

SET(PORT_SOURCE_FILES
	soc.c
	unix.c
	${FREERTOS_SOURCE_FILES}
)

add_library(lwiot-platform ${PORT_SOURCE_FILES})
target_include_directories(lwiot-platform PUBLIC ${PORT_INCLUDE_DIR})
