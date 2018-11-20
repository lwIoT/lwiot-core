#
# FreeRTOS build file.
#

SET(RTOS_DIR ${PROJECT_SOURCE_DIR}/source/platform/ports/freertos)
if(UNIX)
SET(FREERTOS_SOURCE_FILES ${RTOS_DIR}/rtos.c ${RTOS_DIR}/unix.c)
else()
SET(FREERTOS_SOURCE_FILES ${RTOS_DIR}/rtos.c)
endif()

