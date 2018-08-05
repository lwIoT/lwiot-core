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

#include <driver/i2c.h>

namespace lwiot
{
	class Esp32I2CAlgorithm : public I2CAlgorithm {
	public:
		explicit Esp32I2CAlgorithm(int sda, int scl, uint32_t freq = 100000, i2c_port_t portnum = I2C_NUM_0);
		explicit Esp32I2CAlgorithm();
		virtual ~Esp32I2CAlgorithm();

		virtual ssize_t transfer(I2CMessage& msg) override;
		virtual ssize_t transfer(Vector<I2CMessage*>& msgs) override;
		virtual void setFrequency(const uint32_t& freq) override;

	private:
		GpioPin _sda;
		GpioPin _scl;
		i2c_port_t _portno;
		i2c_config_t config;

		/* Methods */
		void prepareTransfer(i2c_cmd_handle_t handle, I2CMessage& msg) const;
	};
}
