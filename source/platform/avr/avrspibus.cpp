/*
 * AVR SPI bus.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/io/spibus.h>
#include <lwiot/types.h>
#include <lwiot/log.h>

#include <lwiot/avr/avrspibus.h>
#include <lwiot/avr/avr.h>

#if defined(ARDUINO_MEGA)
#define MOSI 51
#define MISO 50
#define SCK  52
#elif defined( ARDUINO_UNO)
#define MOSI 11
#define MISO 12
#define SCK  13
#endif

namespace lwiot { namespace avr
{
	SpiBus::SpiBus(uint32_t freq) : lwiot::SpiBus(MOSI, MISO, SCK, freq)
	{
		this->_mosi.mode(PinMode::OUTPUT);
		this->_clk.mode(PinMode::OUTPUT);
		this->_miso.mode(PinMode::INPUT);
		this->_mosi.write(false);
		this->_clk.write(false);

		atmega_spi_init();
	}

	SpiBus::~SpiBus()
	{
		atmega_spi_destroy();

		this->_mosi.mode(PinMode::INPUT);
		this->_clk.mode(PinMode::INPUT);
		this->_miso.mode(PinMode::INPUT);
	}

	void SpiBus::setFrequency(uint32_t frequency)
	{
		this->_frequency = frequency;
		atmega_spi_setspeed(frequency);
	}

	bool SpiBus::transfer(const uint8_t* tx, uint8_t* rx, size_t length)
	{
		return static_cast<size_t>(atmega_spi_xfer(tx, rx, length)) == length;
	}
}
}
