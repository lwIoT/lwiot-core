/*
 * Generic digital to analog converter class.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

/// @file dacchip.h

#include <stdlib.h>
#include <stdint.h>
#include <lwiot.h>

#ifdef CXX
namespace lwiot
{
	/**
	 * @brief DAC peripheral abstraction.
	 * @ingroup io
	 */
	class DacChip {
	public:
		virtual ~DacChip() = default; //!< DAC chip destructor.

		/**
		 * @brief Start the DAC peripheral.
		 * @param zeropin Start pin count at zero indicator.
		 */
		virtual void begin(bool zeropin) final;

		/**
		 * @brief Start the DAC chip.
		 */
		virtual void begin();

		virtual void enable(int pin) = 0; //!< Enable the peripheral.
		virtual void disable(int pin) = 0; //!< Disable the peripheral.

		/**
		 * @brief Get the number of pins.
		 * @return Number of DAC pins.
		 */
		virtual const int& pins() const;

		/**
		 * @brief Get the DAC width.
		 * @return Width of the DAC.
		 */
		virtual uint8_t width() const;

		/**
		 * @brief Get the reference voltage.
		 * @return The reference voltage.
		 */
		virtual uint16_t vref() const;

		/**
		 * @brief Write a value to the DAC.
		 * @param pin Pin to write to.
		 * @param voltage Voltage value to set.
		 */
		virtual void write(int pin, const size_t& voltage) = 0;

	protected:
		/**
		 * @brief Create a new DAC object.
		 * @param pins Number of pins.
		 * @param width Width of the DAC.
		 * @param vref Voltage reference.
		 */
		explicit DacChip(int pins, uint8_t width, uint16_t vref);

		DacChip() = default; //!< Default constructor.

		/**
		 * @brief Map a voltage to a DAC value.
		 * @param voltage Voltage to map.
		 * @return Mapped voltage level.
		 */
		virtual size_t map(const size_t& voltage) const;

		bool _zero_pin; //!< Zero pin indicator.

	private:
		int _pins;
		uint8_t _width;
		uint16_t _vref;
	};
}

extern lwiot::DacChip& dac;
#endif
