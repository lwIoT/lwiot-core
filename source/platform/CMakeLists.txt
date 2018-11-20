#
# CMakeLists file for the platform library.
#

if(ESP32)
add_subdirectory(esp32)
elseif(ESP8266)
add_subdirectory(esp8266)
elseif(AVR)
add_subdirectory(avr)
elseif(WIN32)
add_subdirectory(win32)
else()
add_subdirectory(unix)
endif()

SET(SOURCES ${LWIOT_PORT_SRCS})
INSTALL(FILES ${LWIOT_PORT_HEADERS} DESTINATION include)
