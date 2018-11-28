#
# AVR platform build file.
#

include(${PROJECT_SOURCE_DIR}/source/platform/ports/freertos/freertos.cmake)

set(AVR_GENERIC_SOURCES
	${PROJECT_SOURCE_DIR}/source/platform/avr/avr-i2c.c
	${PROJECT_SOURCE_DIR}/source/platform/avr/avrspi.c
	${PROJECT_SOURCE_DIR}/source/platform/avr/gpio/avr.c

	${PROJECT_SOURCE_DIR}/source/platform/avr/avradcchip.cpp
	${PROJECT_SOURCE_DIR}/source/platform/avr/avri2calgorithm.cpp
	${PROJECT_SOURCE_DIR}/source/platform/avr/avrspibus.cpp
	${PROJECT_SOURCE_DIR}/source/platform/avr/avruart.cpp
	${PROJECT_SOURCE_DIR}/source/platform/avr/avrwatchdog.cpp
	${PROJECT_SOURCE_DIR}/source/platform/avr/gpio/avrgpiochip.cpp

	${PROJECT_SOURCE_DIR}/source/platform/avr/soc/avrsystem.cpp
	${PROJECT_SOURCE_DIR}/source/platform/avr/soc/cplusplus.cpp
	${PROJECT_SOURCE_DIR}/source/platform/avr/soc/critical.c
	${PROJECT_SOURCE_DIR}/source/platform/avr/soc/avr.c
)

if(NO_OS)
	set(PORT_SOURCE_FILES
		${PROJECT_SOURCE_DIR}/source/platform/avr/no-os/avr.c
	)
else()
	set(PORT_SOURCE_FILES
		${FREERTOS_SOURCE_FILES}
	)
endif()

add_library(lwiot-platform ${PORT_SOURCE_FILES} ${AVR_GENERIC_SOURCES})

target_include_directories(lwiot-platform PUBLIC ${PROJECT_SOURCE_DIR}/source/platform/avr/include)
target_include_directories(lwiot-platform PUBLIC ${PORT_INCLUDE_DIR})