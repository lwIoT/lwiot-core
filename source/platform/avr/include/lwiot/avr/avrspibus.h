/*
 * AVR SPI bus.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdint.h>
#include <stdlib.h>

#include <lwiot/io/spibus.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int atmega_spi_xfer(const uint8_t *tx, uint8_t *rx, size_t length);
extern int atmega_spi_setspeed(uint32_t rate);
extern void  atmega_spi_init(void);
extern void atmega_spi_destroy(void);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace lwiot { namespace avr
{
	class SpiBus : public lwiot::SpiBus {
	public:
		explicit SpiBus(uint32_t freq);
		virtual ~SpiBus();

		virtual void setFrequency(uint32_t freq) override;
		using lwiot::SpiBus::transfer;

	protected:
		virtual bool transfer(const uint8_t* tx, uint8_t* rx, size_t length) override;

	private:
		uint32_t _frequency;
		uint8_t _num;
		GpioPin _ss;
	};
}
}
#endif
