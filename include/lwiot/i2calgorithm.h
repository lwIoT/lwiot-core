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
#include <lwiot/stl/vector.h>

#define MAX_RETRIES 3

namespace lwiot
{
	class I2CAlgorithm {
	public:
		explicit I2CAlgorithm(int delay, uint32_t frequency);
		explicit I2CAlgorithm();
		virtual ~I2CAlgorithm() = default;

		const uint32_t& frequency() const;
		int delay() const;

		virtual void setFrequency(const uint32_t& freq);

		virtual ssize_t transfer(I2CMessage& msg) = 0;
		virtual ssize_t transfer(stl::Vector<I2CMessage*>& msgs) = 0;

	protected:
		uint32_t _frequency;
		static constexpr int DefaultRetryDelay = 200;

	private:
		int _retry;

		static constexpr uint32_t DefaultFrequency = 100000UL;
	};
}
