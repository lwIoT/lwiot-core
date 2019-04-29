/*
 * I2C serial bus wrapper.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/bytebuffer.h>
#include <lwiot/log.h>
#include <lwiot/error.h>
#include <lwiot/types.h>
#include <lwiot/stl/vector.h>
#include <lwiot/scopedlock.h>

#include <lwiot/io/i2calgorithm.h>
#include <lwiot/io/i2cbus.h>
#include <lwiot/io/i2cmessage.h>

namespace lwiot
{
	I2CBus::I2CBus(const lwiot::I2CBus &bus)
	{
		this->_lock = bus._lock;
		this->_algo = bus._algo;
	}

	I2CBus::I2CBus(I2CAlgorithm *algo) : _algo(algo), _lock(new Lock(false))
	{
	}

	I2CBus::I2CBus() : I2CBus(nullptr)
	{
	}

	I2CBus& I2CBus::operator=(const lwiot::I2CBus &bus)
	{
		this->_lock = bus._lock;
		this->_algo = bus._algo;
		return *this;
	}

	void I2CBus::setFrequency(const uint32_t& freq)
	{
		this->_algo->setFrequency(freq);
	}

	I2CAlgorithm* I2CBus::algorithm() const
	{
		return this->_algo.get();
	}

	bool I2CBus::transfer(stl::Vector<I2CMessage*>& msgs)
	{
		int rv = -EINVALID;
		ScopedLock lock(*this->_lock);

		for(int i = 0; i < MAX_RETRIES; i++) {
			rv = this->_algo->transfer(msgs);

			if(rv == -ETRYAGAIN) {
				lwiot_sleep(this->_algo->delay());
				continue;
			} else {
				break;
			}
		}

		return rv > 0;
	}

	bool I2CBus::transfer(I2CMessage& msg)
	{
		int rv = -EINVALID;
		ScopedLock lock(*this->_lock);

		for(int i = 0; i < MAX_RETRIES; i++) {
			rv = this->_algo->transfer(msg);

			if(rv == -ETRYAGAIN) {
				lwiot_sleep(this->_algo->delay());
				continue;
			} else {
				break;
			}
		}

		return rv > 0;
	}

	void I2CBus::setAlgorithm(I2CAlgorithm* algo)
	{
		this->_algo.reset(algo);
	}

	bool I2CBus::send(int8_t sla, const lwiot::ByteBuffer &buffer)
	{
		I2CMessage msg(buffer);

		msg.setAddress(sla, false, false);
		msg.setRepeatedStart(false);
		return this->transfer(msg);
	}

	bool I2CBus::send(int8_t sla, const void *buffer, size_t size)
	{
		I2CMessage msg(size);

		msg.setAddress(sla, false, false);
		msg.setRepeatedStart(false);
		msg.write(static_cast<const uint8_t *>(buffer), size);
		return this->transfer(msg);
	}
}
