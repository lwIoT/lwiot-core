#
# AVR toolchain file
#

SET(CMAKE_SYSTEM_NAME Generic)

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.6.0")
	set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
else()
	include(CMakeForceCompiler)

	set(XTENSA_GCC_COMPILER "avr-gcc${CMAKE_EXECUTABLE_SUFFIX}")
	set(XTENSA_GXX_COMPILER "avr-g++${CMAKE_EXECUTABLE_SUFFIX}")
	cmake_force_c_compiler(avr-gcc GNU)
	cmake_force_cxx_compiler(avr-g++ GNU)
endif()

set(HAVE_BIG_ENDIAN False)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

SET(NO_OS False CACHE BOOL "Build without RTOS")
SET(MCU "atmega2560" CACHE STRING "AVR MCU type")

if(NO_OS)
SET(LWIOT_PORT_DIR ${PROJECT_SOURCE_DIR}/source/ports/no-os)
set(LWIOT_PORT_INCLUDE_DIRECTORIES
	${PROJECT_SOURCE_DIR}/source/ports/no-os
	${PROJECT_SOURCE_DIR}/include/asm/avr
	${PROJECT_SOURCE_DIR}/source/lib/time
)
SET(RTOS_SOURCES
	${PROJECT_SOURCE_DIR}/source/ports/no-os/avr.c
)
else()
SET(LWIOT_PORT_DIR ${PROJECT_SOURCE_DIR}/source/ports/freertos)
set(LWIOT_PORT_INCLUDE_DIRECTORIES
		${PROJECT_SOURCE_DIR}/source/ports/freertos
		${PROJECT_SOURCE_DIR}/external/avr-freertos/include
		${PROJECT_SOURCE_DIR}/include/asm/avr
)
SET(RTOS_SOURCES
	${PROJECT_SOURCE_DIR}/source/ports/freertos/rtos.c
)
endif()

SET(LWIOT_PORT_SRCS
	${PROJECT_SOURCE_DIR}/source/io/gpio/avr.c
	${PROJECT_SOURCE_DIR}/source/io/gpio/avrgpiochip.cpp
	${PROJECT_SOURCE_DIR}/source/io/adc/avradcchip.cpp
	${PROJECT_SOURCE_DIR}/source/io/uart/avruart.cpp
	${PROJECT_SOURCE_DIR}/source/io/wdt/avrwatchdog.cpp

	${PROJECT_SOURCE_DIR}/source/io/spi/avr/avrspi.c
	${PROJECT_SOURCE_DIR}/source/io/i2c/avr/avr-i2c.c
	${PROJECT_SOURCE_DIR}/source/io/i2c/avr/avri2calgorithm.cpp

	${PROJECT_SOURCE_DIR}/source/soc/avr.c
	${PROJECT_SOURCE_DIR}/source/soc/avrsystem.cpp
	${PROJECT_SOURCE_DIR}/source/soc/cplusplus.cpp
	${RTOS_SOURCES}
)

SET(LWIOT_PORT_HEADERS
		${LWIOT_PORT_DIR}/lwiot_arch.h
)

set(PORT_C_FLAGS "-mmcu=${MCU}")
set(PORT_CXX_FLAGS "-mmcu=${MCU} -fno-rtti -std=gnu++14")

SET(HAVE_LIBTIME True)
