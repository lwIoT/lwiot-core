/*
 * I2C serial bus wrapper.
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
#include <lwiot/kernel/lock.h>
#include <lwiot/sharedpointer.h>

#include <lwiot/io/i2chal.h>
#include <lwiot/io/i2calgorithm.h>

namespace lwiot
{
	class I2CBus {
	public:
		explicit I2CBus();
		I2CBus(const I2CBus& bus);
		explicit I2CBus(I2CAlgorithm *algo);

		I2CBus& operator=(const I2CBus& bus);

		bool transfer(I2CMessage& msg);
		bool transfer(stl::Vector<I2CMessage>& msgs);

		bool send(int8_t  sla, const ByteBuffer& buffer);
		bool send(int8_t sla, const void *buffer, size_t size);

		void setFrequency(const uint32_t& freq);
		void setAlgorithm(I2CAlgorithm* algo);

		I2CAlgorithm *algorithm() const;

		inline int timeout() const
		{
			return this->_timeout;
		}

		inline void setTimeout(int tmo)
		{
			this->_timeout = tmo;
		}

	private:
		SharedPointer<I2CAlgorithm> _algo;
		SharedPointer<Lock> _lock;
		int _timeout;
	};
}
