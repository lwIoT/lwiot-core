/*
 * Analog digital converter chip.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file adcchip.h

#pragma once

#include <stdlib.h>
#include <stdio.h>

namespace lwiot
{
	/**
	 * @brief ADC peripheral abstraction.
	 * @ingroup io
	 */
	class AdcChip {
	public:
		virtual void begin() = 0; //!< Start the ADC.
		virtual ~AdcChip() = default;

		/**
		 * @brief Read from an ADC pin.
		 * @param pin Pin to read from.
		 * @return Analog value on \p pin.
		 */
		virtual size_t read(int pin) const = 0;

		/**
		 * @brief Convert a reading to a voltage level based on the chip reference voltage.
		 * @param reading ADC reading to convert.
		 * @return \f$ \frac{xV_ref}{W} \f$
		 */
		size_t toVoltage(const size_t& reading) const;

		/**
		 * @brief Get the number of pins.
		 * @return Number of ADC pins.
		 */
		virtual const int& pins() const;

		/**
		 * @brief Read operator.
		 * @param pin Pin to rad.
		 * @return Value ot \p pin.
		 * @see read
		 */
		size_t operator [](int pin) const;

	protected:
		/**
		 * @brief Create a new ADC chip.
		 * @param pins Number of pins.
		 * @param ref Reference voltage.
		 * @param width ADC width.
		 */
		explicit AdcChip(int pins, int ref, int width);

	private:
		int _pins;
		int _reference;
		int _width;
	};
}

extern lwiot::AdcChip& adc;
