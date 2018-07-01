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

SET(NO_OS False CACHE BOOL "Build without RTOS")
SET(MCU "atmega2560" CACHE STRING "AVR MCU type")

if(NO_OS)
set(LWIOT_PORT_INCLUDE_DIRECTORIES
	${PROJECT_SOURCE_DIR}/source/ports/no-os
	${PROJECT_SOURCE_DIR}/include/asm/avr
	${PROJECT_SOURCE_DIR}/source/lib/time
)
SET(RTOS_SOURCES
	${PROJECT_SOURCE_DIR}/source/ports/no-os/avr.c
)
else()
set(LWIOT_PORT_INCLUDE_DIRECTORIES
		${PROJECT_SOURCE_DIR}/source/ports/freertos
		${PROJECT_SOURCE_DIR}/external/avr-freertos/include
		${PROJECT_SOURCE_DIR}/include/asm/avr
)
SET(RTOS_SOURCES
	${PROJECT_SOURCE_DIR}/source/ports/freertos/rtos.c
)
endif()

SET(LWIOT_PORT_DIR ${PROJECT_SOURCE_DIR}/source/ports/freertos)
SET(LWIOT_PORT_SRCS
	${PROJECT_SOURCE_DIR}/source/io/gpio/avr.c
	${PROJECT_SOURCE_DIR}/source/io/gpio/avrgpiochip.cpp
	${PROJECT_SOURCE_DIR}/source/io/adc/avradcchip.cpp
	${PROJECT_SOURCE_DIR}/source/io/uart/avruart.cpp
	${PROJECT_SOURCE_DIR}/source/io/wdt/avrwatchdog.cpp
	${PROJECT_SOURCE_DIR}/source/soc/avr.c
	${PROJECT_SOURCE_DIR}/source/soc/avrsystem.cpp
	${PROJECT_SOURCE_DIR}/source/soc/cplusplus.cpp
	${RTOS_SOURCES}
)

SET(LWIOT_PORT_HEADERS
		${LWIOT_PORT_DIR}/lwiot_arch.h
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mmcu=${MCU}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mmcu=${MCU} -fno-rtti")

SET(HAVE_LIBTIME True)
