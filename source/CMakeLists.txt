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
	kernel/thread.cpp
	kernel/functionalthread.cpp
	kernel/lock.cpp
	kernel/event.cpp
	kernel/timer.cpp
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

SET(GENERIC_HEADERS
	lwiot.h
	ArduinoJson.h
	lwiot/functor.h
	lwiot/compiler-gcc.h
	lwiot/test.h
	lwiot/lwiot.h
	lwiot/types.h
	lwiot/function.h
	lwiot/compiler.h
	lwiot/log.h
	lwiot/gfxbase.h
	lwiot/realtimeclock.h
	lwiot/bufferedstream.h
	lwiot/countable.h
	lwiot/scopedlock.h
	lwiot/printer.h
	lwiot/uniquepointer.h
	lwiot/system.h
	lwiot/stream.h
	lwiot/sharedpointer.h
	lwiot/compiler-vc.h
	lwiot/error.h
	lwiot/bytebuffer.h
	lwiot/network/httpserver.h
	lwiot/network/dns.h
	lwiot/network/udpclient.h
	lwiot/network/ipaddress.h
	lwiot/network/stdnet.h
	lwiot/network/tcpclient.h
	lwiot/network/requesthandler.h
	lwiot/network/sockettcpserver.h
	lwiot/network/wifistation.h
	lwiot/network/socketudpserver.h
	lwiot/network/securetcpclient.h
	lwiot/network/sockettcpclient.h
	lwiot/network/captiveportal.h
	lwiot/network/base64.h
	lwiot/network/wifiaccesspoint.h
	lwiot/network/tcpserver.h
	lwiot/network/udpserver.h
	lwiot/network/socketudpclient.h
	lwiot/network/xbee/constants.h
	lwiot/network/xbee/xbeeresponse.h
	lwiot/network/xbee/xbeeaddress.h
	lwiot/network/xbee/xbee.h
	lwiot/network/xbee/xbeerequest.h
	lwiot/io/spibus.h
	lwiot/io/adcpin.h
	lwiot/io/watchdog.h
	lwiot/io/pwm.h
	lwiot/io/i2cmessage.h
	lwiot/io/uart.h
	lwiot/io/spimessage.h
	lwiot/io/gpiochip.h
	lwiot/io/i2calgorithm.h
	lwiot/io/dhtbus.h
	lwiot/io/onewirebus.h
	lwiot/io/adcchip.h
	lwiot/io/dacpin.h
	lwiot/io/gpioi2calgorithm.h
	lwiot/io/dacchip.h
	lwiot/io/i2cbus.h
	lwiot/io/gpiopin.h
	lwiot/device/bmpsensor.h
	lwiot/device/eeprom24c02.h
	lwiot/device/dsrealtimeclock.h
	lwiot/device/ccs811sensor.h
	lwiot/device/bmp085sensor.h
	lwiot/device/dhtsensor.h
	lwiot/device/ssd1306display.h
	lwiot/device/bmp280sensor.h
	lwiot/device/apds9301sensor.h
	lwiot/stl/vector.h
	lwiot/stl/move.h
	lwiot/stl/forward.h
	lwiot/stl/array.h
	lwiot/stl/map.h
	lwiot/stl/linkedlist.h
	lwiot/stl/tuple.h
	lwiot/stl/container_of.h
	lwiot/stl/string.h
	lwiot/util/guid.h
	lwiot/util/list.h
	lwiot/util/application.h
	lwiot/util/count.h
	lwiot/util/json.h
	lwiot/util/datetime.h
	lwiot/kernel/atomic.h
	lwiot/util/measurementvector.h
	lwiot/util/defaultallocator.h
	lwiot/util/pair.h
	lwiot/kernel/port.h
	lwiot/kernel/event.h
	lwiot/kernel/lock.h
	lwiot/kernel/functionalthread.h
	lwiot/kernel/timer.h
	lwiot/kernel/thread.h
	lwiot/traits/integralconstant.h
	lwiot/traits/isreference.h
	lwiot/traits/isintegral.h
	lwiot/traits/removereference.h
	lwiot/traits/isfloatingpoint.h
	lwiot/traits/enableif.h
	lwiot/traits/removecv.h
	lwiot/traits/typechoice.h
	lwiot/traits/issame.h
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

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	target_compile_options(lwiot PUBLIC -pedantic-errors)
endif()

CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/scripts/config.h.in ${CMAKE_BINARY_DIR}/lwiot_opts.h)

foreach(source IN LISTS ${SOURCES})
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "Source Files\\${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

SET(PUB_HDRS "")
LIST(APPEND PUB_HDRS "${GENERIC_HEADERS}")

# Install targets
INSTALL(TARGETS lwiot
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)
INSTALL(FILES ${CMAKE_BINARY_DIR}/lwiot_opts.h DESTINATION include)
