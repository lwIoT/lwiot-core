#
# I/O subsystem build file.
#

SET(DIR ${PROJECT_SOURCE_DIR}/source/io)

if(HAVE_UNISTD_H)
	SET(FILEIO_SRC io/fs/file.cpp)
endif()

SET(IO_SOURCES
	io/gpio/gpiochip.cpp
	io/gpio/gpiopin.cpp

	io/adc/adcchip.cpp
	io/adc/adcpin.cpp
	io/adc/dacchip.cpp
	io/adc/dacpin.cpp

	io/uart/uart.cpp

	io/pwm/pwmchannel.cpp
	io/pwm/pwmtimer.cpp
	io/pwm/leddriver.cpp
	io/pwm/rgbleddriver.cpp

	io/onewire/onewirebus.cpp
	io/onewire/dhtbus.cpp

	io/wdt/watchdog.cpp

	io/spi/spimessage.cpp
	io/spi/spibus.cpp

	io/i2c/i2cmessage.cpp
	io/i2c/i2calgorithm.cpp
	io/i2c/hardwarei2calgorithm.cpp
	io/i2c/i2cbus.cpp
	io/i2c/gpioi2calgorithm.cpp

	${FILEIO_SRC}
)
