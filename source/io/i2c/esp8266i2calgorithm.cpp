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
#include <lwiot/i2cmessage.h>
#include <lwiot/vector.h>
#include <lwiot/i2calgorithm.h>
#include <lwiot/gpiopin.h>

#include <lwiot/esp8266/esp8266i2calgorithm.h>

#include <i2c/i2c.h>

namespace lwiot
{
	Esp8266I2CAlgorithm::Esp8266I2CAlgorithm(int sda, int scl, uint32_t freq, uint8_t bus) :
		I2CAlgorithm(), _sda(sda), _scl(scl), _bus(bus)
	{
		if(i2c_init_hz(bus, scl, sda, freq)) {
			print_dbg("Unable to initialise I2C bus!\n");
		}
	}

	Esp8266I2CAlgorithm::Esp8266I2CAlgorithm() : Esp8266I2CAlgorithm(4, 3)
	{
	}

	Esp8266I2CAlgorithm::~Esp8266I2CAlgorithm()
	{
		this->_sda.input();
		this->_scl.input();
	}

	void Esp8266I2CAlgorithm::setFrequency(const uint32_t& freq)
	{
		I2CAlgorithm::setFrequency(freq);
	}

	ssize_t Esp8266I2CAlgorithm::transfer(I2CMessage& msg)
	{
		uint8_t addr, data;
		size_t count;

		enter_critical();
		i2c_start(this->_bus);

		addr = (uint8_t)msg.address();
		addr <<= 1;

		if(msg.isRead())
			addr |= 1;
		
		if(!i2c_write(this->_bus, addr)) {
			print_dbg("[i2c]: Unable to write address!\n");
			i2c_stop(this->_bus);
			exit_critical();
			return -EINVALID;
		}

		if(msg.isRead()) {
			count = msg.count();
			for(auto idx = 0UL; idx < count - 1; idx++) {
				data = i2c_read(this->_bus, false);
				msg[idx] = data;
			}

			data = i2c_read(this->_bus, true);
			msg[count - 1] = data;
			msg.setIndex(count);
		} else {
			for(auto& byte : msg) {
				if(!i2c_write(this->_bus, byte)) {
					i2c_stop(this->_bus);
					exit_critical();
					return -EINVALID;
				}
			}
		}

		if(!msg.repstart())
			i2c_stop(this->_bus);

		exit_critical();
		return static_cast<ssize_t>(msg.count());
	}

	ssize_t Esp8266I2CAlgorithm::transfer(Vector<I2CMessage*>& msgs)
	{
		ssize_t total;
		ssize_t rv;

		total = 0L;

		for(auto _msg : msgs) {
			auto& msg = *_msg;

			rv = this->transfer(msg);
			if(rv < 0)
				return rv;

			total += rv;
		}

		return total;
	}
}