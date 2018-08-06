/*
 * ESP32 I2C algorithm implementation.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/lwiot.h>
#include <lwiot/bytebuffer.h>
#include <lwiot/log.h>
#include <lwiot/error.h>
#include <lwiot/types.h>
#include <lwiot/i2cmessage.h>
#include <lwiot/vector.h>
#include <lwiot/i2calgorithm.h>
#include <lwiot/gpiopin.h>

namespace lwiot
{
	class Esp8266I2CAlgorithm : public I2CAlgorithm {
	public:
		explicit Esp8266I2CAlgorithm(int sda, int scl, uint32_t freq = 100000, uint8_t bus = 0);
		explicit Esp8266I2CAlgorithm();
		virtual ~Esp8266I2CAlgorithm();

		virtual ssize_t transfer(I2CMessage& msg) override;
		virtual ssize_t transfer(Vector<I2CMessage*>& msgs) override;
		virtual void setFrequency(const uint32_t& freq) override;

	private:
		GpioPin _sda;
		GpioPin _scl;
		uint8_t _bus;
	};
}