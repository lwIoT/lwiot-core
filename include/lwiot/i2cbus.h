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

namespace lwiot
{
	class I2CBus {
	public:
		explicit I2CBus(I2CAlgorithm& algo);
		explicit I2CBus(I2CAlgorithm *algo);

		bool transfer(I2CMessage& msg);
		bool transfer(Vector<I2CMessage*>& msgs);

		void setFrequency(const uint32_t& freq);
		void setAlgorithm(I2CAlgorithm& algo);

	private:
		I2CAlgorithm& _algo;
	};
}
