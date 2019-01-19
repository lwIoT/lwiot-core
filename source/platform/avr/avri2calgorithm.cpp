/*
 * ESP32 I2C algorithm implementation.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <lwiot/lwiot.h>
#include <lwiot/bytebuffer.h>
#include <lwiot/log.h>
#include <lwiot/error.h>
#include <lwiot/types.h>
#include <lwiot/io/i2cmessage.h>
#include <lwiot/stl/vector.h>
#include <lwiot/io/i2calgorithm.h>
#include <lwiot/io/gpiopin.h>

#include <lwiot/avr/avri2calgorithm.h>

#include "avri2c.h"

#ifdef ARDUINO_UNO
#define SCL 5
#define SDA 4
#elif defined(ARDUINO_MEGA)
#define SCL 21
#define SDA 20
#endif

namespace lwiot { namespace avr
{
	I2CAlgorithm::I2CAlgorithm() : 
		lwiot::I2CAlgorithm(lwiot::I2CAlgorithm::DefaultRetryDelay, 100000U), _sda(SDA), _scl(SCL)
	{
		avr_i2c_init();
	}

	void I2CAlgorithm::setFrequency(const uint32_t& freq)
	{
		/* Variadic frequencies not yet supported. */
	}

	ssize_t I2CAlgorithm::transfer(I2CMessage& msg)
	{
		return this->transfer(msg, true);
	}

	ssize_t I2CAlgorithm::transfer(stl::Vector<I2CMessage*>& msgs)
	{
		ssize_t retval, total;

		total = 0L;

		for(auto _msg : msgs) {
			auto& msg = *_msg;

			retval = this->transfer(msg, !msg.repstart());

			if(retval > 0) {
				total += retval;
			} else {
				if(msg.repstart())
					i2c_stop();

				return retval;
			}
		}

		return total;
	}

	ssize_t I2CAlgorithm::transfer(I2CMessage& msg, bool stop)
	{
		uint8_t *data, addr, rv;
		size_t length;

		data = msg.data();
		length = msg.count();
		addr = static_cast<uint8_t>(msg.address() & 0xFF);
		addr <<= 1U; /* Make room for directional bit */

		if(msg.isRead()) {
			rv = i2c_receive(addr, data, length, stop);
			msg.setIndex(length);
		} else {
			rv = i2c_transmit(addr, data, length, stop);
		}

		return (rv == 0) ? static_cast<ssize_t>(length) : -EINVALID;
	}
}}
