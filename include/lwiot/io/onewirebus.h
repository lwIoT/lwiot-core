/*
 * One wire protocol bus definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file onewirebus.h

#pragma once

#include <stdlib.h>

#include <lwiot/lwiot.h>
#include <lwiot/io/gpiopin.h>
#include <lwiot/log.h>
#include <lwiot/io/gpiochip.h>
#include <lwiot/bufferedstream.h>
#include <lwiot/kernel/lock.h>

typedef uint64_t onewire_addr_t;

namespace lwiot
{
	/**
	 * @brief OneWire protocol implementation.
	 * @ingroup io
	 */
	class OneWireBus {
	public:
		/**
		 * @brief Construct a new OneWireBus object.
		 * @param pin Data pin.
		 */
		explicit OneWireBus(const GpioPin& pin);
		virtual ~OneWireBus(); //!< OneWireBus destructor.

		/**
		 * @brief Reset the bus.
		 * @return Success indicator.
		 */
		bool reset();

		/**
		 * @brief Select a device.
		 * @param addr Device address.
		 * @return Success indicator.
		 */
		bool select(const onewire_addr_t& addr);

		/**
		 * @brief Write data to a device.
		 * @param data Data to write.
		 * @return Success indicator.
		 */
		bool write(BufferedStream& data);

		/**
		 * @brief Write data to a device.
		 * @param data Data to write.
		 * @param length Data length.
		 * @return Success indicator.
		 */
		bool write(const uint8_t *data, size_t length);

		/**
		 * @brief Read data from a device.
		 * @param output Input buffer.
		 * @param length Length of the input buffer.
		 * @return A success indicator.
		 */
		bool read(BufferedStream& output, size_t length);

		/**
		 * @brief Read data from a device.
		 * @param data Input buffer.
		 * @param length Length of the input buffer.
		 * @return A success indicator.
		 */
		bool read(uint8_t *data, size_t length);

		/**
		 * @brief Data pin getter.
		 * @return The data pin.
		 */
		const GpioPin& pin() const;

	protected:
		/**
		 * @brief Write a single byte.
		 * @param value Byte to write.
		 * @return A success indicator.
		 */
		bool write(uint8_t value);

		/**
		 * @brief Read a single byte.
		 * @param value Byte read from the one-wire bus.
		 * @return A success indicator.
		 */
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
