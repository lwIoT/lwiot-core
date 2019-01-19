/*
 * ESP32 SPI bus implementation.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>
#include <esp_system.h>

#include <lwiot/io/spibus.h>
#include <lwiot/io/spimessage.h>
#include <lwiot/log.h>

#include <lwiot/esp32/esp32spibus.h>

#include <driver/gpio.h>
#include <driver/spi_master.h>

#include "esp32spi.h"

#define SPI_MSBFIRST 1
#define SPI_MODE0    0

namespace lwiot { namespace esp32
{
	SpiBus::SpiBus(uint8_t num, uint8_t clk, uint8_t miso, uint8_t mosi) :
		lwiot::SpiBus(mosi, miso, clk, 1000000UL), _frequency(1000000UL), _num(num)
	{
		this->_div = spiFrequencyToClockDiv(this->_frequency);
		this->_spi = spiStartBus(num, this->_div, SPI_MODE0, SPI_MSBFIRST);

		spiAttachSCK(this->_spi, miso);
		spiAttachMISO(this->_spi, miso);
		spiAttachMOSI(this->_spi, miso);
	}

	SpiBus::~SpiBus()
	{
		spiDetachMOSI(this->_spi, this->_mosi.pin());
		spiDetachMISO(this->_spi, this->_miso.pin());
		spiDetachSCK(this->_spi, this->_clk.pin());
		spiStopBus(this->_spi);
	}

	void SpiBus::setFrequency(uint32_t freq)
	{
		auto clkdiv = spiFrequencyToClockDiv(freq);

		this->_frequency = freq;

		if(this->_div == clkdiv)
			return;

		spiSetClockDiv(this->_spi, clkdiv);
		this->_div = clkdiv;
	}

	bool SpiBus::transfer(const uint8_t* tx, uint8_t* rx, size_t length)
	{
		spiTransferBytes(this->_spi, (uint8_t*)tx, rx, length);
		return true;
	}
}
}
