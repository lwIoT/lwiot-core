#
# I/O subsystem build file.
#

SET(DIR ${PROJECT_SOURCE_DIR}/source/io)

SET(IO_SOURCES
	io/gpio/gpiochip.cpp
	io/gpio/gpiopin.cpp

	io/adc/adcchip.cpp
	io/adc/adcpin.cpp
	io/adc/dacchip.cpp
	io/adc/dacpin.cpp

	io/uart/uart.cpp
)

SET(CONFIG_PIN_VECTOR False CACHE BOOL "Build a vector of pins the the GPIO chip.")
