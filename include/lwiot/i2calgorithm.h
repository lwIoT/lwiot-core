/*
 * I2C algorithm base class.
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

namespace lwiot
{
	class I2CAlgorithm {
	public:
		explicit I2CAlgorithm(int delay, uint32_t frequency);
		explicit I2CAlgorithm();

		const uint32_t& frequency() const;
		void setFrequency(const uint32_t& freq);

		virtual ssize_t transfer(const I2CMessage& msg) = 0;
		virtual ssize_t transfer(const Vector<I2CMessage>& msgs) = 0;

	protected:
		uint32_t _frequency;

	private:
		int _retry;

		static constexpr uint32_t DefaultFrequency = 100000UL;
		static constexpr int DefaultRetryDelay = 200;
	};
}
