/*
 * Abstract SPI bus definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot.h>
#include <stdlib.h>

#include <lwiot/spimessage.h>
#include <lwiot/types.h>
#include <lwiot/gpiopin.h>
#include <lwiot/vector.h>

#ifdef __cplusplus
namespace lwiot
{
	class SpiBus {
	public:
		explicit SpiBus(int mosi, int miso, int clk, uint32_t freq = 1000000UL);
		virtual ~SpiBus() = default;

		const uint32_t& frequency() const;
		const GpioPin& miso() const;
		const GpioPin& mosi() const;
		const GpioPin& clk() const;

		virtual bool transfer(SpiMessage& msg);
		virtual bool transfer(Vector<SpiMessage>& msgs);
		virtual void setFrequency(uint32_t freq) = 0;

	protected:
		GpioPin _miso;
		GpioPin _mosi;
		GpioPin _clk;

		virtual bool transfer(const uint8_t *tx, uint8_t *rx, size_t length) = 0;

	private:
		uint32_t _freq;
	};
}
#endif
