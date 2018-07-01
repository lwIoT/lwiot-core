#
# Source CMakeLists.txt
#

include(CheckIncludeFiles)
include(CheckTypeSize)
include(CheckCSourceCompiles)
include(${PROJECT_SOURCE_DIR}/source/io/io.cmake)
include(${PROJECT_SOURCE_DIR}/source/lib/lib.cmake)

include( ${PROJECT_SOURCE_DIR}/cmake/port.cmake)

if(NOT NO_OS)
SET(WRAPPER_SOURCES
	ports/thread.cpp
	ports/lock.cpp
	ports/event.cpp
	ports/timer.cpp
)
else()
SET(WRAPPER_SOURCES )
endif()

SET(SOURCES
    init.c
    log.c
	bytebuffer.cpp
    datetime.cpp
	log.cpp
    string.cpp
	vector.cpp
    system.cpp
    ${WRAPPER_SOURCES}
	${IO_SOURCES}
    ${LWIOT_PORT_SRCS}
    ${LIB_SOURCES}
)

set(GENERIC_HEADERS
    asm/avr/cpu.h
    asm/esp32/cpu.h
    asm/esp8266/cpu.h
    asm/hosted/cpu.h
    lwiot/adcchip.h
    lwiot/adcpin.h
    lwiot/avr/avradcchip.h
    lwiot/avr/avrgpiochip.h
    lwiot/avr/avrsystem.h
    lwiot/avr/avruart.h
    lwiot/avr/avrwatchdog.h
    lwiot/bufferedstream.h
    lwiot/bytebuffer.h
    lwiot/compiler-gcc.h
    lwiot/compiler-vc.h
    lwiot/compiler.h
    lwiot/countable.h
    lwiot/dacchip.h
    lwiot/dacpin.h
    lwiot/error.h
    lwiot/esp32/esp32adcchip.h
    lwiot/esp32/esp32dacchip.h
    lwiot/esp32/esp32gpiochip.h
    lwiot/esp32/esp32primaryadc.h
    lwiot/esp32/esp32secondaryadc.h
    lwiot/esp32/esp32system.h
    lwiot/esp32/esp32uart.h
    lwiot/esp32/esp32watchdog.h
    lwiot/esp8266/esp8266adcchip.h
    lwiot/esp8266/esp8266gpiochip.h
    lwiot/esp8266/esp8266system.h
    lwiot/esp8266/esp8266uart.h
    lwiot/esp8266/esp8266watchdog.h
    lwiot/event.h
    lwiot/gpiochip.h
    lwiot/gpiopin.h
    lwiot/hosted/hostedgpiochip.h
    lwiot/ipaddress.h
    lwiot/json.h
    lwiot/list.h
    lwiot/lock.h
    lwiot/log.h
    lwiot/lwiot.h
    lwiot/pair.h
    lwiot/port.h
    lwiot/stream.h
    lwiot/string.h
    lwiot/system.h
    lwiot/test.h
    lwiot/thread.h
    lwiot/timer.h
    lwiot/types.h
    lwiot/uart.h
    lwiot/vector.h
    lwiot/watchdog.h
    lwiot/wifiaccesspoint.h
    lwiot/wifistation.h
    lwiot.h
)

# Include timer implementation on UNIX and Windows builds
IF(NOT CMAKE_SYSTEM_NAME MATCHES Generic)
SET(SOURCES
${SOURCES}
ports/timer.c)
ENDIF()

set(BASE_HDRS
    ${PROJECT_SOURCE_DIR}/include/lwiot.h
    ${PROJECT_BINARY_DIR}/lwiot_opts.h
)

SET(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

FUNCTION( prepend_path SOURCE_FILES INC_PATH )
    FOREACH( SOURCE_FILE ${${SOURCE_FILES}} )
        SET( MODIFIED ${MODIFIED} ${INC_PATH}/${SOURCE_FILE} )
    ENDFOREACH()
    SET( ${SOURCE_FILES} ${MODIFIED} PARENT_SCOPE )
ENDFUNCTION()

prepend_path(GENERIC_HEADERS ${INCLUDE_DIR})

include_directories(
    ${PROJECT_SOURCE_DIR}/include ${CMAKE_BINARY_DIR} ${EXTRA_INCLUDE_DIRECTORIES}
    ${LWIOT_PORT_INCLUDE_DIRECTORIES}
)

if(EXTRA_LINK_DIRECTORIES)
    link_directories(${EXTRA_LINK_DIRECTORIES})
endif()

SET(${SOURCES} ${SOURCES})

add_library(lwiot ${SOURCES} ${GENERIC_HEADERS} ${BASE_HDRS} ${LWIOT_PORT_HEADERS})
link_directories(${PROJECT_SOURCE_DIR}/external/freertos/lib)

CONFIGURE_FILE(${INCLUDE_DIR}/lwiot/config.h.in ${CMAKE_BINARY_DIR}/lwiot_opts.h)

foreach(source IN LISTS ${SOURCES})
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "Source Files\\${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

# Install targets
INSTALL(TARGETS lwiot DESTINATION lib)
INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)
INSTALL(FILES ${LWIOT_PORT_HEADERS} DESTINATION include)
INSTALL(FILES ${CMAKE_BINARY_DIR}/lwiot_opts.h DESTINATION include)
