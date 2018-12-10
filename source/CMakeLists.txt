#
# Source CMakeLists.txt
#

include(CheckIncludeFiles)
include(CheckTypeSize)
include(CheckCSourceCompiles)
include(${PROJECT_SOURCE_DIR}/source/io/io.cmake)
include(${PROJECT_SOURCE_DIR}/source/lib/lib.cmake)
include(${PROJECT_SOURCE_DIR}/source/sensors/sensors.cmake)
include(${PROJECT_SOURCE_DIR}/source/net/net.cmake)

include( ${PROJECT_SOURCE_DIR}/cmake/port.cmake)

if(NOT NO_OS)
SET(WRAPPER_SOURCES
	ports/thread.cpp
	ports/functionalthread.cpp
	ports/lock.cpp
	ports/event.cpp
	ports/timer.cpp
)
else()
SET(WRAPPER_SOURCES )
endif()

SET(SOURCES
    init.c
    application.cpp

    util/log.c
    util/bytebuffer.cpp
    util/datetime.cpp
    util/log.cpp
    util/scopedlock.cpp
    util/string.cpp
    util/vector.cpp
    util/system.cpp
    ${WRAPPER_SOURCES}
	${IO_SOURCES}
    ${LIB_SOURCES}
    ${SENSOR_SOURCES}
)

if(HAVE_NETWORKING)
	set(SOURCES
		${SOURCES}
		${NET_SOURCES}
)
endif()

set(GENERIC_HEADERS
	lwiot.h
	ArduinoJson.h

	lwiot/network/tcpclient.h
	lwiot/network/tcpserver.h
	lwiot/network/ipaddress.h
	lwiot/network/wifiaccesspoint.h
	lwiot/network/wifistation.h

	lwiot/httpserver.h

	lwiot/requesthandler.h
	lwiot/base64.h
	lwiot/sharedpointer.h
	lwiot/uniquepointer.h
	lwiot/apds9301sensor.h
	lwiot/measurementvector.h
	lwiot/application.h
	lwiot/adcchip.h
	lwiot/adcpin.h
	lwiot/bufferedstream.h
	lwiot/bytebuffer.h
	lwiot/compiler.h
	lwiot/compiler-gcc.h
	lwiot/compiler-vc.h
	lwiot/config.h.in
	lwiot/count.h
	lwiot/countable.h
	lwiot/dacchip.h
	lwiot/dacpin.h
	lwiot/datetime.h
	lwiot/dhtbus.h
	lwiot/dhtsensor.h
	lwiot/eeprom24c02.h
	lwiot/error.h
	lwiot/event.h
	lwiot/functionalthread.h
	lwiot/gpiochip.h
	lwiot/gpiopin.h
	lwiot/i2calgorithm.h
	lwiot/gpioi2calgorithm.h
	lwiot/realtimeclock.h
	lwiot/dsrealtimeclock.h
	lwiot/atomic.h
	lwiot/i2cbus.h
	lwiot/i2cmessage.h
	lwiot/json.h
	lwiot/list.h
	lwiot/lock.h
	lwiot/log.h
	lwiot/lwiot.h
	lwiot/onewirebus.h
	lwiot/pair.h
	lwiot/port.h
	lwiot/pwm.h
	lwiot/scopedlock.h
	lwiot/spibus.h
	lwiot/spimessage.h
	lwiot/ssd1306display.h
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
	lwiot/printer.h
	lwiot/gfxbase.h
	lwiot/bmpsensor.h
	lwiot/bmp085sensor.h
	lwiot/bmp280sensor.h
	lwiot/ccs811sensor.h
	lwiot/function.h
)

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
	${LWIOT_CORE_INCLUDE_DIRECTORIES}
)

SET(${SOURCES} ${SOURCES})

add_subdirectory(${PLATFORM_DIRECTORY})
add_library(lwiot ${SOURCES} ${GENERIC_HEADERS} ${BASE_HDRS} )

CONFIGURE_FILE(${INCLUDE_DIR}/lwiot/config.h.in ${CMAKE_BINARY_DIR}/lwiot_opts.h)

foreach(source IN LISTS ${SOURCES})
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "Source Files\\${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()


# Install targets
INSTALL(TARGETS lwiot DESTINATION lib)
INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)
INSTALL(FILES ${CMAKE_BINARY_DIR}/lwiot_opts.h DESTINATION include)
