/*
 * ESP32 SPI bus implementation.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <stdio.h>

#include <lwiot/io/spibus.h>
#include <lwiot/io/spimessage.h>
#include <lwiot/log.h>

#include <driver/gpio.h>
#include <driver/spi_master.h>

struct spi_struct_t;
typedef struct spi_struct_t spi_t;

#define HSPI 2
#define VSPI 3

namespace lwiot { namespace esp32
{
	class SpiBus : public lwiot::SpiBus {
	public:
		explicit SpiBus(uint8_t num, uint8_t clk, uint8_t miso, uint8_t mosi);
		virtual ~SpiBus();

		virtual void setFrequency(uint32_t freq) override;
		using lwiot::SpiBus::transfer;

	protected:
		virtual bool transfer(const uint8_t* tx, uint8_t* rx, size_t length) override;

	private:
		uint32_t _frequency;
		uint8_t _num;
		uint32_t _div;
		bool _updated;
		spi_t *_spi;
		GpioPin _ss;
	};
}
}
