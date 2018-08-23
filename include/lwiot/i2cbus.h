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
#include <lwiot/i2cmessage.h>
#include <lwiot/vector.h>
#include <lwiot/i2calgorithm.h>
#include <lwiot/lock.h>

namespace lwiot
{
	class I2CBus {
	public:
		explicit I2CBus(I2CAlgorithm& algo);
		explicit I2CBus(I2CAlgorithm *algo);

		bool transfer(I2CMessage& msg);
		bool transfer(Vector<I2CMessage*>& msgs);

		bool send(int8_t  sla, ByteBuffer& buffer);
		bool send(int8_t sla, void *buffer, size_t size);

		void setFrequency(const uint32_t& freq);
		void setAlgorithm(I2CAlgorithm& algo);

	private:
		I2CAlgorithm& _algo;
		Lock _lock;
	};
}
