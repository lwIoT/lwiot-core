/*
 * Analog digital converter interface.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdio.h>
#include <lwiot.h>

#include <lwiot/adcchip.h>

namespace lwiot
{
	AdcChip::AdcChip(int pins, int ref, int width) : _pins(pins), _reference(ref), _width(width)
	{ }

	const int& AdcChip::pins() const
	{
		return this->_pins;
	}

	size_t AdcChip::toVoltage(const size_t& reading) const
	{
		float ref = this->_reference;
		float width = this->_width;

		return (ref / width) * reading;
	}

	size_t AdcChip::operator[] (int pin) const
	{
		return this->read(pin);
	}
}
