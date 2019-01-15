/*
 * I2C algorithm base class.
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
#include <lwiot/stl/vector.h>
#include <lwiot/i2calgorithm.h>

namespace lwiot
{

		I2CAlgorithm::I2CAlgorithm(int delay, uint32_t frequency) :
			_frequency(frequency), _retry(delay)
		{
		}

		I2CAlgorithm::I2CAlgorithm() :
			I2CAlgorithm(I2CAlgorithm::DefaultRetryDelay, I2CAlgorithm::DefaultFrequency)
		{
		}

		const uint32_t& I2CAlgorithm::frequency() const
		{
			return this->_frequency;
		}

		int I2CAlgorithm::delay() const
		{
			return this->_retry;
		}

		void I2CAlgorithm::setFrequency(const uint32_t& freq)
		{
			this->_frequency = freq;
		}
}
