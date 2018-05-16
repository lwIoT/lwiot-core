set(CMAKE_SYSTEM_NAME "Generic")

include(CMakeForceCompiler)

set(XTENSA_GCC_COMPILER "xtensa-esp32-elf-gcc${CMAKE_EXECUTABLE_SUFFIX}")
set(XTENSA_GXX_COMPILER "xtensa-esp32-elf-g++${CMAKE_EXECUTABLE_SUFFIX}")
set(HAVE_BIG_ENDIAN False)

if(NOT DEFINED ESP32_TOOLCHAIN_PATH)
    # Check if GCC is reachable.
    find_path(ESP32_TOOLCHAIN_PATH bin/${XTENSA_GCC_COMPILER})

    if(NOT ESP32_TOOLCHAIN_PATH)
        # Set default path.
        set(ESP32_TOOLCHAIN_PATH /opt/Espressif/crosstool-NG/builds/xtensa-esp32-elf)
        message(STATUS "GCC not found, default path will be used: ${ESP32_TOOLCHAIN_PATH}")
    endif()
else()
    message(STATUS "Toolchain path: ${ESP32_TOOLCHAIN_PATH}")
endif()

SET(CMAKE_C_COMPILER ${XTENSA_GCC_COMPILER} )
SET(CMAKE_CXX_COMPILER ${XTENSA_GXX_COMPILER} )

#cmake_force_c_compiler(${ESP32_TOOLCHAIN_PATH}/bin/${XTENSA_GCC_COMPILER} GNU)
#cmake_force_cxx_compiler(${ESP32_TOOLCHAIN_PATH}/bin/${XTENSA_GXX_COMPILER} GNU)

set(CMAKE_OBJCOPY ${ESP32_TOOLCHAIN_PATH}/bin/xtensa-esp32-elf-objcopy CACHE PATH "")

#########################
### ESP32 SDK setup ###
#########################

if(NOT DEFINED ESP32_IDF_PATH)
    set(ESP32_IDF_PATH /opt/Espressif/esp32-idf)
    message(STATUS "Default SDK location will be used: ${ESP32_IDF_PATH}")
else()
    message(STATUS "ESP32 SDK path: ${ESP32_IDF_PATH}")
endif()

set(CMAKE_LIBRARY_PATH ${ESP32_IDF_PATH}/lib/)

set(LWIOT_PORT_INCLUDE_DIRECTORIES
        ${PROJECT_SOURCE_DIR}/source/ports/freertos

		${ESP32_IDF_PATH}/components/cxx/include
		${ESP32_IDF_PATH}/components/driver/include
		${ESP32_IDF_PATH}/components/esp32/include
		${ESP32_IDF_PATH}/components/freertos/include
		${ESP32_IDF_PATH}/components/freertos/include/freertos
		${ESP32_IDF_PATH}/components/heap/include
		${ESP32_IDF_PATH}/components/lwip/include/lwip
		${ESP32_IDF_PATH}/components/lwip/include/lwip/port
		${ESP32_IDF_PATH}/components/lwip/include/lwip/posix
		${ESP32_IDF_PATH}/components/lwip/apps/ping
		${ESP32_IDF_PATH}/components/mbedtls/port/include
		${ESP32_IDF_PATH}/components/mbedtls/include
		${ESP32_IDF_PATH}/components/mdns/include
		${ESP32_IDF_PATH}/components/newlib/platform_include
		${ESP32_IDF_PATH}/components/newlib/include
		${ESP32_IDF_PATH}/components/soc/esp32/include
		${ESP32_IDF_PATH}/components/soc/include
		${ESP32_IDF_PATH}/esp32-config
        )

SET(LWIOT_PORT_DIR ${PROJECT_SOURCE_DIR}/source/ports/freertos)
SET(LWIOT_PORT_SRCS
	${LWIOT_PORT_DIR}/rtos.c
	soc/esp32.c
	${PROJECT_SOURCE_DIR}/source/io/gpio/esp32gpiochip.cpp
)

SET(LWIOT_PORT_HEADERS
        ${LWIOT_PORT_DIR}/lwiot_arch.h)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-comment -Wno-pointer-sign -fno-builtin -Wno-implicit-function-declaration \
    -Wl,-EL,--gc-sections -fno-inline-functions -nostdlib -mlongcalls -mtext-section-literals \
    -ffunction-sections" CACHE FORCE "")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -Wno-comment -fno-builtin -Wl,-EL,--gc-sections -fno-inline-functions \
    -nostdlib -mlongcalls -mtext-section-literals -ffunction-sections" CACHE FORCE "")

SET(HAVE_RTOS True)
