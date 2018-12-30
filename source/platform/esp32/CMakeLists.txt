#
# ESP32 platform build file.
#

include(${PROJECT_SOURCE_DIR}/source/platform/ports/freertos/freertos.cmake)

SET(PORT_SOURCE_FILES
	esp32adcchip.cpp
	esp32dacchip.cpp
	esp32gpiochip.cpp
	esp32i2calgorithm.cpp
	esp32primaryadc.cpp
	esp32pwmchannel.cpp
	esp32pwmtimer.cpp
	esp32secondaryadc.cpp
	esp32spi.c
	esp32spi.h
	esp32spibus.cpp
	esp32uart.cpp
	esp32watchdog.cpp
	esp32_ap.cpp
	esp32_station.cpp
	esp32_wifi.c

	soc/esp32.c
	soc/esp32system.cpp
	${FREERTOS_SOURCE_FILES}
	${PROJECT_SOURCE_DIR}/source/platform/lwip/lwip.c
	${PROJECT_SOURCE_DIR}/source/platform/ssl/mbedtls.c
)

add_library(lwiot-platform ${PORT_SOURCE_FILES})

target_include_directories(lwiot-platform PUBLIC ${PROJECT_SOURCE_DIR}/source/platform/esp32/include)
target_include_directories(lwiot-platform PUBLIC ${PORT_INCLUDE_DIR})
