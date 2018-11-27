#
# AVR platform build file.
#

include(${PROJECT_SOURCE_DIR}/source/platform/ports/freertos/freertos.cmake)

set(PORT_SOURCE_FILES
	${PROJECT_SOURCE_DIR}/source/platform/avr/avr-i2c.c
	${PROJECT_SOURCE_DIR}/source/platform/avr/avrspi.c
	${PROJECT_SOURCE_DIR}/source/platform/avr/gpio/avr.c
	${PROJECT_SOURCE_DIR}/source/platform/avr/soc/avr.c

	${PROJECT_SOURCE_DIR}/source/platform/avr/avradcchip.cpp
	${PROJECT_SOURCE_DIR}/source/platform/avr/avri2calgorithm.cpp
	${PROJECT_SOURCE_DIR}/source/platform/avr/avrspibus.cpp
	${PROJECT_SOURCE_DIR}/source/platform/avr/avruart.cpp
	${PROJECT_SOURCE_DIR}/source/platform/avr/avrwatchdog.cpp
	${PROJECT_SOURCE_DIR}/source/platform/avr/gpio/avrgpiochip.cpp
	${PROJECT_SOURCE_DIR}/source/platform/avr/soc/avrsystem.cpp
	${PROJECT_SOURCE_DIR}/source/platform/avr/soc/cplusplus.cpp
	${FREERTOS_SOURCE_FILES}

	#${PROJECT_SOURCE_DIR}/source/platform/avr/no-os/avr.c
)

add_library(lwiot-platform ${PORT_SOURCE_FILES})

target_include_directories(lwiot-platform PUBLIC ${PROJECT_SOURCE_DIR}/source/platform/avr/include)
target_include_directories(lwiot-platform PUBLIC ${PORT_INCLUDE_DIR})