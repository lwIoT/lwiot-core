set(CMAKE_SYSTEM_NAME "Generic")

include(CMakeForceCompiler)

set(XTENSA_GCC_COMPILER "xtensa-lx106-elf-gcc${CMAKE_EXECUTABLE_SUFFIX}")
set(XTENSA_GXX_COMPILER "xtensa-lx106-elf-g++${CMAKE_EXECUTABLE_SUFFIX}")
set(HAVE_BIG_ENDIAN False)

if(NOT DEFINED ESP8266_TOOLCHAIN_PATH)
    # Check if GCC is reachable.
    find_path(ESP8266_TOOLCHAIN_PATH bin/${XTENSA_GCC_COMPILER})

    if(NOT ESP8266_TOOLCHAIN_PATH)
        # Set default path.
        set(ESP8266_TOOLCHAIN_PATH /opt/Espressif/crosstool-NG/builds/xtensa-lx106-elf)
        message(STATUS "GCC not found, default path will be used: ${ESP8266_TOOLCHAIN_PATH}")
    endif()
else()
    message(STATUS "Toolchain path: ${ESP8266_TOOLCHAIN_PATH}")
endif()

SET(CMAKE_C_COMPILER ${XTENSA_GCC_COMPILER} )
SET(CMAKE_CXX_COMPILER ${XTENSA_GXX_COMPILER} )

#cmake_force_c_compiler(${ESP8266_TOOLCHAIN_PATH}/bin/${XTENSA_GCC_COMPILER} GNU)
#cmake_force_cxx_compiler(${ESP8266_TOOLCHAIN_PATH}/bin/${XTENSA_GXX_COMPILER} GNU)

set(CMAKE_OBJCOPY ${ESP8266_TOOLCHAIN_PATH}/bin/xtensa-lx106-elf-objcopy CACHE PATH "")

#########################
### ESP8266 SDK setup ###
#########################

if(NOT DEFINED ESP8266_OPEN_RTOS_PATH)
    set(ESP8266_SDK_PATH /opt/Espressif/esp-rtos-sdk)
    message(STATUS "Default SDK location will be used: ${ESP8266_OPEN_RTOS_PATH}")
else()
    message(STATUS "ESP8266 SDK path: ${ESP8266_OPEN_RTOS_PATH}")
endif()

set(CMAKE_LIBRARY_PATH ${ESP8266_OPEN_RTOS_PATH}/lib/)

set(LWIOT_PORT_INCLUDE_DIRECTORIES
		${PROJECT_SOURCE_DIR}/include/asm/esp8266
        ${PROJECT_SOURCE_DIR}/source/ports/freertos
        ${ESP8266_OPEN_RTOS_PATH}/include
        ${ESP8266_OPEN_RTOS_PATH}/libc/xtensa-lx106-elf/include
        ${ESP8266_OPEN_RTOS_PATH}/FreeRTOS/Source/include
        ${ESP8266_OPEN_RTOS_PATH}/FreeRTOS/Source/portable/esp8266
        ${ESP8266_OPEN_RTOS_PATH}/lwip/lwip/src/include
        ${ESP8266_OPEN_RTOS_PATH}/lwip/include
        ${ESP8266_OPEN_RTOS_PATH}/lwip/lwip/src/include/compat/posix
        ${ESP8266_OPEN_RTOS_PATH}/lwip/lwip/src/include/ipv4
        ${ESP8266_OPEN_RTOS_PATH}/lwip/lwip/src/include/ipv4/lwip
        ${ESP8266_OPEN_RTOS_PATH}/lwip/lwip/src/include/lwip
        ${ESP8266_OPEN_RTOS_PATH}/core/include
        ${ESP8266_OPEN_RTOS_PATH}/open_esplibs/include
        )

SET(LWIOT_PORT_DIR ${PROJECT_SOURCE_DIR}/source/ports/freertos)
SET(LWIOT_PORT_SRCS
	${LWIOT_PORT_DIR}/rtos.c
    ${PROJECT_SOURCE_DIR}/source/io/gpio/esp8266gpiochip.cpp
    ${PROJECT_SOURCE_DIR}/source/io/adc/esp8266adcchip.cpp
    ${PROJECT_SOURCE_DIR}/source/io/uart/esp8266uart.cpp
    ${PROJECT_SOURCE_DIR}/source/io/uart/esp8266-softuart.c
    ${PROJECT_SOURCE_DIR}/source/soc/esp8266system.cpp
    ${PROJECT_SOURCE_DIR}/source/io/wdt/esp8266watchdog.cpp
    ${PROJECT_SOURCE_DIR}/source/io/wifi/esp8266_station.cpp
    ${PROJECT_SOURCE_DIR}/source/io/wifi/esp8266_ap.cpp
	${PROJECT_SOURCE_DIR}/source/soc/cplusplus.cpp
	${PROJECT_SOURCE_DIR}/source/soc/esp8266.c
)

SET(ESP8266 True CACHE BOOL "ESP8266 target.")

SET(LWIOT_PORT_HEADERS
        ${LWIOT_PORT_DIR}/lwiot_arch.h
)

set(CMAKE_C_FLAGS "-Wno-comment -Wno-pointer-sign -fno-builtin -Wno-implicit-function-declaration \
    -Wl,-EL,--gc-sections -fno-inline-functions -nostdlib -mlongcalls -mtext-section-literals \
    -ffunction-sections" CACHE FORCE "")
set(CMAKE_CXX_FLAGS "-fno-rtti -fno-exceptions -Wno-comment -fno-builtin -Wl,-EL,--gc-sections -fno-inline-functions \
    -nostdlib -mlongcalls -mtext-section-literals -ffunction-sections" CACHE FORCE "")

SET(HAVE_RTOS True)
SET(HAVE_JSON True)
