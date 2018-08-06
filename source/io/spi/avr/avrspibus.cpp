/*
 * AVR SPI bus.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/spibus.h>
#include <lwiot/types.h>
#include <lwiot/log.h>

#include <lwiot/avr/avrspibus.h>

#ifdef ARDUINO_MEGA
#define MOSI 51
#define MISO 50
#define SCK  52
#elif ARDUINO_UNO
#define MOSI 11
#define MISO 12
#define SCK  13
#endif

namespace lwiot { namespace avr
{
	AvrSpiBus::AvrSpiBus(uint32_t freq) : lwiot::SpiBus(MOSI, MISO, SCK, freq)
	{
		this->_mosi.mode(PinMode::OUTPUT);
		this->_clk.mode(PinMode::OUTPUT);
		this->_miso.mode(PinMode::INPUT);
		this->_mosi.write(false);
		this->_clk.write(false);

		atmega_spi_init();
	}

	AvrSpiBus::~AvrSpiBus()
	{
		atmega_spi_destroy();

		this->_mosi.mode(PinMode::INPUT);
		this->_clk.mode(PinMode::INPUT);
		this->_miso.mode(PinMode::INPUT);
	}

	void AvrSpiBus::setFrequency(uint32_t frequency)
	{
		this->_frequency = frequency;
		atmega_spi_setspeed(frequency);
	}

	bool AvrSpiBus::transfer(const uint8_t* tx, uint8_t* rx, size_t length)
	{
		atmega_spi_xfer(tx, rx, length);
	}
}
}
