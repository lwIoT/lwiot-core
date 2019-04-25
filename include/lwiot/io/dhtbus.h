/*
 * DHT humidity 1-wire protocol bus.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/lwiot.h>
#include <lwiot/io/gpiopin.h>
#include <lwiot/log.h>
#include <lwiot/stl/vector.h>
#include <lwiot/kernel/lock.h>

namespace lwiot
{
	struct DhtReadout {
		float humidity;
		float temperature;
	};

	class DhtBus {
	public:
		explicit DhtBus(const GpioPin& pin);
		virtual ~DhtBus();

		bool read(stl::Vector<bool>& output);
		const GpioPin& pin() const;

		static constexpr int Bits = 40;

	private:
		GpioPin _pin;
		Lock _lock;

		static constexpr int Interval = 1;

		static constexpr int PhaseOne = 40;
		static constexpr int PhaseTwo = 88;
		static constexpr int PhaseThree = 88;

		static constexpr int DelayOne = 56;
		static constexpr int DelayTwo = 75;


		/* Methods */
		bool await(uint32_t tmo, bool expected, uint32_t& duration);
		bool _read(stl::Vector<bool>& output);
	};
}
