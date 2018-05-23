SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERISON 1)

SET(CMAKE_C_COMPILER avr-gcc)
SET(CMAKE_CXX_COMPILER avr-g++)
set(HAVE_BIG_ENDIAN False)


# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

SET(MCU "atmega2560" CACHE STRING "AVR MCU type")


set(LWIOT_PORT_INCLUDE_DIRECTORIES
		${PROJECT_SOURCE_DIR}/source/ports/freertos
		${PROJECT_SOURCE_DIR}/external/avr-freertos/include
)

SET(LWIOT_PORT_DIR ${PROJECT_SOURCE_DIR}/source/ports/freertos)
SET(LWIOT_PORT_SRCS
	${LWIOT_PORT_DIR}/rtos.c
	${PROJECT_SOURCE_DIR}/source/io/gpio/avr.c
	${PROJECT_SOURCE_DIR}/source/io/gpio/avrgpiochip.cpp
	${PROJECT_SOURCE_DIR}/source/io/adc/avradcchip.cpp
	${PROJECT_SOURCE_DIR}/source/soc/avr.c
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mmcu=${MCU}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mmcu=${MCU}")
