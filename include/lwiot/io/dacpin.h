/*
 * Digital to analog conversion pin
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file dacpin.h

#pragma once

#include <stdlib.h>
#include <stdio.h>

#include <lwiot/lwiot.h>
#include <lwiot/io/dacchip.h>

namespace lwiot
{
	/**
	 * @brief DAC pin abstraction.
	 * @ingroup io
	 */
	class DacPin {
	public:
		/**
		 * @brief Construct a new DAC pin.
		 * @param pin Pin number.
		 */
		explicit DacPin(int pin);

		/**
		 * @brief Construct a new DAC pin.
		 * @param pin Pin number.
		 * @param chip DAC chip.
		 */
		explicit DacPin(int pin, DacChip& chip);
		virtual ~DacPin() = default; //!< DAC pin destructor.

		/**
		 * @brief Write a value (voltage level) to the DAC.
		 * @param value Value (voltage) to write.
		 */
		void write(const size_t& value);

	private:
		DacChip& _chip;
		int _pin;
	};
}
