#
# ESP32 build file.
#
# @Author: Michel Megens
# @Email:  dev@bietje.net
#

SET(CMAKE_SYSTEM_NAME Generic)

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.6.0")
    set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
else()
	include(CMakeForceCompiler)

	set(XTENSA_GCC_COMPILER "xtensa-esp32-elf-gcc${CMAKE_EXECUTABLE_SUFFIX}")
	set(XTENSA_GXX_COMPILER "xtensa-esp32-elf-g++${CMAKE_EXECUTABLE_SUFFIX}")
	cmake_force_c_compiler(xtensa-esp32-elf-gcc GNU)
	cmake_force_cxx_compiler(xtensa-esp32-elf-g++ GNU)
endif()

set(HAVE_BIG_ENDIAN False)

if(NOT DEFINED ESP32_TOOLCHAIN_PATH)
    # Set default path.
    set(ESP32_TOOLCHAIN_PATH /opt/xtensa-esp32-elf)
	SET(CMAKE_FIND_ROOT_PATH  /opt/xtensa-esp32-elf)
    message(STATUS "GCC not found, default path will be used: ${ESP32_TOOLCHAIN_PATH}")
else()
    message(STATUS "Toolchain path: ${ESP32_TOOLCHAIN_PATH}")
	SET(CMAKE_FIND_ROOT_PATH ${ESP32_TOOLCHAIN_PATH})
endif()


SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_OBJCOPY ${ESP32_TOOLCHAIN_PATH}/bin/xtensa-esp32-elf-objcopy CACHE PATH "")

#########################
### ESP32 SDK setup ###
#########################

if(NOT DEFINED ESP32_IDF_PATH)
    set(ESP32_IDF_PATH ${PROJECT_SOURCE_DIR}/external/esp32-idf)
    message(STATUS "Default SDK location will be used: ${ESP32_IDF_PATH}")
else()
    message(STATUS "ESP32 SDK path: ${ESP32_IDF_PATH}")
endif()

set(CMAKE_LIBRARY_PATH ${ESP32_IDF_PATH}/lib/)
SET(CONFIG_APP_CFG_PATH ${PROJECT_SOURCE_DIR}/usr/esp32_test CACHE STRING "Application Kconfig directory")

if(NOT IS_ABSOLUTE ${CONFIG_APP_CFG_PATH})
	SET(APP_CONFIG "${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_APP_CFG_PATH}")
else()
	SET(APP_CONFIG "${CONFIG_APP_CFG_PATH}")
endif()

message( STATUS "Config path: ${APP_CONFIG}")

set(LWIOT_PORT_INCLUDE_DIRECTORIES
        ${PROJECT_SOURCE_DIR}/source/ports/freertos
		${PROJECT_SOURCE_DIR}/include/asm/esp32

		${ESP32_IDF_PATH}/components/cxx/include
		${ESP32_IDF_PATH}/components/driver/include
		${ESP32_IDF_PATH}/components/esp32/include
		${ESP32_IDF_PATH}/components/freertos/include
		${ESP32_IDF_PATH}/components/heap/include
		${ESP32_IDF_PATH}/components/nvs_flash/include
		${ESP32_IDF_PATH}/components/lwip/include/lwip
		${ESP32_IDF_PATH}/components/lwip/include/lwip/port
		${ESP32_IDF_PATH}/components/lwip/include/lwip/posix
		${ESP32_IDF_PATH}/components/lwip/apps/ping
		${ESP32_IDF_PATH}/components/mbedtls/port/include
		${ESP32_IDF_PATH}/components/mbedtls/include
		${ESP32_IDF_PATH}/components/mdns/include
		${ESP32_IDF_PATH}/components/newlib/platform_include
		${ESP32_IDF_PATH}/components/newlib/include
		${ESP32_IDF_PATH}/components/vfs/include
		${ESP32_IDF_PATH}/components/tcpip_adapter/include
		${ESP32_IDF_PATH}/components/soc/esp32/include
		${ESP32_IDF_PATH}/components/soc/include
		${ESP32_IDF_PATH}/components/freertos/include/freertos
		${APP_CONFIG}/build/include
)

SET(LWIOT_PORT_DIR ${PROJECT_SOURCE_DIR}/source/ports/freertos)
SET(LWIOT_PORT_SRCS
	${LWIOT_PORT_DIR}/rtos.c
	${PROJECT_SOURCE_DIR}/source/soc/esp32.c
	${PROJECT_SOURCE_DIR}/source/soc/esp32system.cpp
	${PROJECT_SOURCE_DIR}/source/io/wifi/esp32_station.cpp
	${PROJECT_SOURCE_DIR}/source/io/wifi/esp32_ap.cpp
	${PROJECT_SOURCE_DIR}/source/io/wifi/esp32_wifi.c
	${PROJECT_SOURCE_DIR}/source/io/gpio/esp32gpiochip.cpp
	${PROJECT_SOURCE_DIR}/source/io/adc/esp32dacchip.cpp
	${PROJECT_SOURCE_DIR}/source/io/adc/esp32adcchip.cpp
	${PROJECT_SOURCE_DIR}/source/io/adc/esp32primaryadc.cpp
	${PROJECT_SOURCE_DIR}/source/io/adc/esp32secondaryadc.cpp

	${PROJECT_SOURCE_DIR}/source/io/uart/esp32uart.cpp
	${PROJECT_SOURCE_DIR}/source/io/wdt/esp32watchdog.cpp

	${PROJECT_SOURCE_DIR}/source/io/pwm/esp32/esp32pwmchannel.cpp
	${PROJECT_SOURCE_DIR}/source/io/pwm/esp32/esp32pwmtimer.cpp

	${PROJECT_SOURCE_DIR}/source/io/spi/esp32/esp32spibus.cpp
	${PROJECT_SOURCE_DIR}/source/io/spi/esp32/esp32spi.c

	${PROJECT_SOURCE_DIR}/source/io/i2c/esp32/esp32i2calgorithm.cpp
)

SET(LWIOT_PORT_HEADERS
		${LWIOT_PORT_DIR}/lwiot_arch.h)
		
SET(ESP32 True CACHE BOOL "Build for the ESP32 SoC.")

SET(HAVE_RTOS True)
SET(HAVE_JSON True)
SET(HAVE_LWIP True)
SET(HAVE_NETWORKING True)
SET(RTTI False CACHE BOOL "Enable/disable runtime type identification.")

IF(RTTI)
	set(PORT_CXX_FLAGS "-frtti -Wno-comment -fno-builtin \
	-Wl,-EL,--gc-sections -fno-inline-functions -nostdlib -mlongcalls \
	-mtext-section-literals -ffunction-sections -fdata-sections")
ELSE()
	set(PORT_CXX_FLAGS "-fno-rtti -Wno-comment -fno-builtin \
	-Wl,-EL,--gc-sections -fno-inline-functions -nostdlib -mlongcalls \
	-mtext-section-literals -ffunction-sections -fdata-sections")
ENDIF()

set(PORT_C_FLAGS "-Wno-comment -Wno-pointer-sign -fno-builtin \
	-Wno-implicit-function-declaration -Wl,-EL,--gc-sections -fno-inline-functions \
	-nostdlib -mlongcalls -mtext-section-literals \
    -ffunction-sections -fdata-sections" )

