set(CMAKE_SYSTEM_NAME "Generic")

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.6.0")
    set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
else()
    include(CMakeForceCompiler)

    set(XTENSA_GCC_COMPILER "xtensa-lx106-elf-gcc${CMAKE_EXECUTABLE_SUFFIX}")
    set(XTENSA_GXX_COMPILER "xtensa-lx106-elf-g++${CMAKE_EXECUTABLE_SUFFIX}")
    cmake_force_c_compiler(xtensa-lx106-elf-gcc GNU)
    cmake_force_cxx_compiler(xtensa-lx106-elf-g++ GNU)
endif()

set(HAVE_BIG_ENDIAN False)

if(NOT DEFINED ESP8266_TOOLCHAIN_PATH)
# Set default path.
    set(ESP8266_TOOLCHAIN_PATH /opt/esp8266-open-sdk/xtensa-lx106-elf)
    message(STATUS "GCC not found, default path will be used: ${ESP8266_TOOLCHAIN_PATH}")
else()
    message(STATUS "Toolchain path: ${ESP8266_TOOLCHAIN_PATH}")
endif()

set(CMAKE_OBJCOPY ${ESP8266_TOOLCHAIN_PATH}/bin/xtensa-lx106-elf-objcopy CACHE PATH "")

#########################
### ESP8266 SDK setup ###
#########################

if(NOT DEFINED ESP8266_OPEN_RTOS_PATH)
    set(ESP8266_OPEN_RTOS_PATH ${PROJECT_SOURCE_DIR}/external/esp8266-open-rtos)
    message(STATUS "Default SDK location will be used: ${ESP8266_OPEN_RTOS_PATH}")
else()
    message(STATUS "ESP8266 SDK path: ${ESP8266_OPEN_RTOS_PATH}")
endif()

set(CMAKE_LIBRARY_PATH ${ESP8266_OPEN_RTOS_PATH}/lib/)

set(PORT_INCLUDE_DIR
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
    ${ESP8266_OPEN_RTOS_PATH}/extras
    ${ESP8266_OPEN_RTOS_PATH}/extras/mbedtls/mbedtls/include
    ${ESP8266_OPEN_RTOS_PATH}/extras/mbedtls/include
    ${ESP8266_OPEN_RTOS_PATH}/open_esplibs/include

    ${ESP8266_OPEN_RTOS_PATH}/lwip/lwip/src/include
    ${ESP8266_OPEN_RTOS_PATH}/lwip/include
    ${ESP8266_OPEN_RTOS_PATH}/lwip/lwip/src/include/compat/posix
    ${ESP8266_OPEN_RTOS_PATH}/lwip/lwip/src/include/ipv4
    ${ESP8266_OPEN_RTOS_PATH}/lwip/lwip/src/include/ipv4/lwip
    ${ESP8266_OPEN_RTOS_PATH}/lwip/lwip/src/include/lwip

    ${PROJECT_SOURCE_DIR}/source/platform/esp8266/include
)

set(LWIOT_CORE_INCLUDE_DIRECTORIES
    ${ESP8266_OPEN_RTOS_PATH}/include
    ${ESP8266_OPEN_RTOS_PATH}/libc/xtensa-lx106-elf/include
    ${ESP8266_OPEN_RTOS_PATH}/core/include
    ${ESP8266_OPEN_RTOS_PATH}/open_esplibs/include
)

SET(PLATFORM_DIRECTORY ${PROJECT_SOURCE_DIR}/source/platform/esp8266)

set(PORT_C_FLAGS "${CMAKE_C_FLAGS} -Wno-comment -Wno-pointer-sign -fno-builtin -Wno-implicit-function-declaration \
    -Wl,-EL,--gc-sections -fno-inline-functions -nostdlib -mlongcalls -mtext-section-literals \
    -ffunction-sections -fdata-sections")
set(PORT_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-exceptions -Wno-comment -fno-builtin -Wl,-EL,--gc-sections -fno-inline-functions \
    -nostdlib -mlongcalls -mtext-section-literals -ffunction-sections -fdata-sections -Wc++11-compat")

SET(ESP8266 True)
SET(HAVE_RTOS True)
SET(HAVE_JSON True)
SET(HAVE_LWIP True)
SET(HAVE_SYNC_FETCH True)
SET(HAVE_NETWORKING True)
