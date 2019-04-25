/*
 * Analog digital converter chip.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <stdio.h>

namespace lwiot
{
	class AdcChip {
	public:
		virtual void begin() = 0;
		virtual ~AdcChip() = default;

		virtual size_t read(int pin) const = 0;
		size_t toVoltage(const size_t& reading) const;
		virtual const int& pins() const;

		size_t operator [](int pin) const;

	protected:
		explicit AdcChip(int pins, int ref, int width);

	private:
		int _pins;
		int _reference;
		int _width;
	};
}

extern lwiot::AdcChip& adc;
