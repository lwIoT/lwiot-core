
include(${PROJECT_SOURCE_DIR}/source/platform/ports/freertos/freertos.cmake)
SET(PORT_SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/source/platform/esp8266/esp8266gpiochip.cpp
    ${PROJECT_SOURCE_DIR}/source/platform/esp8266/esp8266adcchip.cpp
    ${PROJECT_SOURCE_DIR}/source/platform/esp8266/esp8266uart.cpp
    ${PROJECT_SOURCE_DIR}/source/platform/esp8266/esp8266-softuart.c
    ${PROJECT_SOURCE_DIR}/source/platform/esp8266/esp8266system.cpp
    ${PROJECT_SOURCE_DIR}/source/platform/esp8266/esp8266watchdog.cpp
    ${PROJECT_SOURCE_DIR}/source/platform/esp8266/esp8266_station.cpp
    ${PROJECT_SOURCE_DIR}/source/platform/esp8266/esp8266_ap.cpp

	${PROJECT_SOURCE_DIR}/source/platform/esp8266/soc/esp8266.c
	${PROJECT_SOURCE_DIR}/source/platform/esp8266/soc/cplusplus.cpp
	${PROJECT_SOURCE_DIR}/source/platform/lwip/lwip.c
	${PROJECT_SOURCE_DIR}/source/platform/ssl/mbedtls.c
	${FREERTOS_SOURCE_FILES}
)

add_library(lwiot-platform ${PORT_SOURCE_FILES})

target_include_directories(lwiot-platform PUBLIC ${PROJECT_SOURCE_DIR}/source/platform/esp8266/include)
target_include_directories(lwiot-platform PUBLIC ${PORT_INCLUDE_DIR})
