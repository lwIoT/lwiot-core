/*
 * Digital to Analog converter.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <lwiot.h>

#include <lwiot/io/dacchip.h>

namespace lwiot
{
	DacChip::DacChip(int pins, uint8_t width, uint16_t vref) : _pins(pins), _width(width), _vref(vref)
	{ }

	void DacChip::begin(bool zeropin)
	{
		this->_zero_pin = zeropin;
	}

	void DacChip::begin()
	{
		this->begin(true);
	}

	const int& DacChip::pins() const
	{
		return this->_pins;
	}

	uint8_t DacChip::width() const
	{
		return this->_width;
	}

	uint16_t DacChip::vref() const
	{
		return this->_vref;
	}

	size_t DacChip::map(const size_t& volts) const
	{
		unsigned long value = 1UL;
		double val;
		double voltage = static_cast<double>(volts);
		double ref = static_cast<double>(this->_vref);

		value <<= this->_width;
		value -= 1;
		val = static_cast<double>(value);

		val = ((val * voltage) / ref) - (voltage / ref);
		val += 1.0f;

		return static_cast<size_t>(val);
	}
}
