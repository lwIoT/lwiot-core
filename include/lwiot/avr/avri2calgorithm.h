/*
 * ESP32 I2C algorithm implementation.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <lwiot/lwiot.h>
#include <lwiot/error.h>
#include <lwiot/log.h>
#include <lwiot/types.h>
#include <lwiot/i2cmessage.h>
#include <lwiot/vector.h>
#include <lwiot/i2calgorithm.h>
#include <lwiot/gpiopin.h>

#pragma once

namespace lwiot { namespace avr
{
	class I2CAlgorithm : public lwiot::I2CAlgorithm {
	public:
		explicit I2CAlgorithm();
		virtual ~I2CAlgorithm() = default;

		virtual ssize_t transfer(I2CMessage& msg) override;
		virtual ssize_t transfer(Vector<I2CMessage*>& msgs) override;
		virtual void setFrequency(const uint32_t& freq) override;

	private:
		GpioPin _sda;
		GpioPin _scl;

		/* Methods */
		ssize_t transfer(I2CMessage& msg, bool stop);
	};
}
}