/*
 * One wire protocol bus definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>

#include <lwiot/lwiot.h>
#include <lwiot/gpiopin.h>
#include <lwiot/log.h>
#include <lwiot/gpiochip.h>
#include <lwiot/bufferedstream.h>
#include <lwiot/kernel/lock.h>

typedef uint64_t onewire_addr_t;

namespace lwiot
{
	class OneWireBus {
	public:
		explicit OneWireBus(const GpioPin& pin);
		virtual ~OneWireBus();

		bool reset();
		bool select(const onewire_addr_t& addr);

		bool write(BufferedStream& data);
		bool write(const uint8_t *data, size_t length);

		bool read(BufferedStream& output, size_t length);
		bool read(uint8_t *data, size_t length);

		const GpioPin& pin() const;

	protected:
		bool write(uint8_t value);
		bool read(uint8_t& value);

	private:
		GpioPin _pin;
		Lock _lock;
		mutable Logger _log;

		/* Methods */
		int read(bool& output);
		bool write(bool value);
		bool wait(unsigned long us);

		static constexpr uint8_t msb()
		{
			return 7U;
		}

		static constexpr uint8_t lsbval()
		{
			return 1U;
		}
	};
}
