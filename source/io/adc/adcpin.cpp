/*
 * ADC pin implementation
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>

#include <lwiot/io/adcchip.h>
#include <lwiot/io/adcpin.h>

namespace lwiot
{
	AdcPin::AdcPin(int pin) : AdcPin(pin, adc)
	{ }

	AdcPin::AdcPin(int pin, AdcChip& chip) : _chip(chip), _pin(pin)
	{ }

	size_t AdcPin::read() const
	{
		return this->_chip.read(this->_pin);
	}

	const int& AdcPin::pin() const
	{
		return this->_pin;
	}

	bool AdcPin::operator ==(const AdcPin& pin)
	{
		return this->_pin == pin.pin();
	}

	bool AdcPin::operator >(const AdcPin& pin)
	{
		return this->_pin > pin.pin();
	}

	bool AdcPin::operator <(const AdcPin& pin)
	{
		return this->_pin < pin.pin();
	}

	bool AdcPin::operator >=(const AdcPin& pin)
	{
		return this->_pin >= pin.pin();
	}

	bool AdcPin::operator <=(const AdcPin& pin)
	{
		return this->_pin <= pin.pin();
	}
}
