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

#########################
###  ESP32 SDK setup  ###
#########################

if(NOT DEFINED ESP32_IDF_PATH)
    set(ESP32_IDF_PATH ${PROJECT_SOURCE_DIR}/external/esp32-idf)
    message(STATUS "Default SDK location will be used: ${ESP32_IDF_PATH}")
else()
    message(STATUS "ESP32 SDK path: ${ESP32_IDF_PATH}")
endif()

set(CMAKE_LIBRARY_PATH ${ESP32_IDF_PATH}/lib)
if(CMAKE_BUILD_TYPE MATCHES Debug)
	SET(DEFAULT_CONFIG_PATH ${PROJECT_SOURCE_DIR}/source/platform/esp32/config/Debug)
else()
	SET(DEFAULT_CONFIG_PATH ${PROJECT_SOURCE_DIR}/source/platform/esp32/config/Release)
endif()

SET(CONFIG_APP_CFG_PATH ${DEFAULT_CONFIG_PATH} CACHE STRING "Application Kconfig directory")

if(NOT IS_ABSOLUTE ${CONFIG_APP_CFG_PATH})
	SET(APP_CONFIG "${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_APP_CFG_PATH}")
else()
	SET(APP_CONFIG "${CONFIG_APP_CFG_PATH}")
endif()
message( STATUS "Config path: ${APP_CONFIG}")

set(CMAKE_OBJCOPY ${ESP32_TOOLCHAIN_PATH}/bin/xtensa-esp32-elf-objcopy CACHE PATH "")

SET(PORT_INCLUDE_DIR
	${PROJECT_SOURCE_DIR}/source/platform/esp32/include

	#${ESP32_IDF_PATH}/components/lwip/lwip/src/include
	#${ESP32_IDF_PATH}/components/lwip/port/esp32/include
	#${ESP32_IDF_PATH}/components/lwip/include
	#${ESP32_IDF_PATH}/components/lwip/include_compat
	#${ESP32_IDF_PATH}/components/lwip/lwip/src/include/posix

	${ESP32_IDF_PATH}/components/lwip/include/lwip
	${ESP32_IDF_PATH}/components/lwip/include/lwip/port
	${ESP32_IDF_PATH}/components/lwip/include/lwip/posix
	${ESP32_IDF_PATH}/components/lwip/apps/ping

	${ESP32_IDF_PATH}/components/cxx/include
	${ESP32_IDF_PATH}/components/driver/include
	${ESP32_IDF_PATH}/components/esp32/include
	${ESP32_IDF_PATH}/components/spi_flash/include
	${ESP32_IDF_PATH}/components/esp_ringbuf/include
	${ESP32_IDF_PATH}/components/freertos/include
	${ESP32_IDF_PATH}/components/heap/include
	${ESP32_IDF_PATH}/components/nvs_flash/include

	${ESP32_IDF_PATH}/components/mbedtls/port/include
	${ESP32_IDF_PATH}/components/mbedtls/mbedtls/include
	${ESP32_IDF_PATH}/components/mdns/include
	${ESP32_IDF_PATH}/components/vfs/include
	${ESP32_IDF_PATH}/components/log/include
	${ESP32_IDF_PATH}/components/wpa_supplicant/port/include
	${ESP32_IDF_PATH}/components/wpa_supplicant/include
	${ESP32_IDF_PATH}/components/smartconfig_ack/include
	${ESP32_IDF_PATH}/components/esp_event/include
	${ESP32_IDF_PATH}/components/tcpip_adapter/include
	${ESP32_IDF_PATH}/components/soc/esp32/include
	${ESP32_IDF_PATH}/components/soc/include
	${ESP32_IDF_PATH}/components/freertos/include/freertos
	${ESP32_IDF_PATH}/components/newlib/platform_include

	${APP_CONFIG}
)

SET(PLATFORM_DIRECTORY ${PROJECT_SOURCE_DIR}/source/platform/esp32)

set(LWIOT_CORE_INCLUDE_DIRECTORIES
	${ESP32_IDF_PATH}/components/newlib/include
)

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
	-Wl,-EL,--gc-sections -fno-inline-functions -nostdlib -mlongcalls -Wc++14-compat \
	-mtext-section-literals -ffunction-sections -fdata-sections -fstack-protector")
ENDIF()

set(PORT_C_FLAGS "-Wno-comment -Wno-pointer-sign -fno-builtin \
	-Wno-implicit-function-declaration -Wl,-EL,--gc-sections -fno-inline-functions \
	-nostdlib -mlongcalls -mtext-section-literals \
    -ffunction-sections -fdata-sections -fstack-protector" )

