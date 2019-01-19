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
#include <lwiot/io/i2cmessage.h>
#include <lwiot/stl/vector.h>
#include <lwiot/io/i2calgorithm.h>
#include <lwiot/io/gpiopin.h>

#pragma once

namespace lwiot { namespace avr
{
	class I2CAlgorithm : public lwiot::I2CAlgorithm {
	public:
		explicit I2CAlgorithm();
		~I2CAlgorithm() override = default;

		ssize_t transfer(I2CMessage& msg) override;
		ssize_t transfer(stl::Vector<I2CMessage*>& msgs) override;
		void setFrequency(const uint32_t& freq) override;

	private:
		GpioPin _sda;
		GpioPin _scl;

		/* Methods */
		ssize_t transfer(I2CMessage& msg, bool stop);
	};
}
}