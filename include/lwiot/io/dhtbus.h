/*
 * DHT humidity 1-wire protocol bus.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file dhtbus.h

#pragma once

#include <lwiot/lwiot.h>
#include <lwiot/io/gpiopin.h>
#include <lwiot/log.h>
#include <lwiot/stl/vector.h>
#include <lwiot/kernel/lock.h>

namespace lwiot
{
	/**
	 * @brief DHT readout.
	 * @ingroup io
	 */
	struct DhtReadout {
		float humidity; //!< RH% value.
		float temperature; //!< Temperature.
	};

	/**
	 * @brief Communication bus for DHT sensors.
	 * @see DhtSensor
	 * @ingroup io
	 */
	class DhtBus {
	public:
		/**
		 * @brief Construct a new DHT bus object.
		 * @param pin Data pin.
		 */
		explicit DhtBus(const GpioPin& pin);
		virtual ~DhtBus(); //!< DHT destructor.

		/**
		 * @brief Read from the bus.
		 * @param output Values read from the DHT device.
		 * @return Success indicator.
		 */
		bool read(stl::Vector<bool>& output);

		/**
		 * @brief GpioPin getter.
		 * @return Get the data pin object.
		 */
		const GpioPin& pin() const;

		static constexpr int Bits = 40; //!< Number of data bits.

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
