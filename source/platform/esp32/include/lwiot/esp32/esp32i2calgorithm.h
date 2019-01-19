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
#include <lwiot/io/i2cmessage.h>
#include <lwiot/stl/vector.h>
#include <lwiot/io/i2calgorithm.h>
#include <lwiot/io/gpiopin.h>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/ringbuf.h>

#include <driver/i2c.h>

namespace lwiot { namespace esp32
{
	class I2CAlgorithm : public lwiot::I2CAlgorithm {
	public:
		explicit I2CAlgorithm(int sda, int scl, uint32_t freq = 100000, i2c_port_t portnum = I2C_NUM_0);
		explicit I2CAlgorithm();
		~I2CAlgorithm() override;

		ssize_t transfer(I2CMessage& msg) override;
		ssize_t transfer(stl::Vector<I2CMessage*>& msgs) override;
		void setFrequency(const uint32_t& freq) override;

	private:
		GpioPin _sda;
		GpioPin _scl;
		i2c_port_t _portno;
		i2c_config_t config;

		/* Methods */
		void prepareTransfer(i2c_cmd_handle_t handle, I2CMessage& msg) const;
	};
}
}
