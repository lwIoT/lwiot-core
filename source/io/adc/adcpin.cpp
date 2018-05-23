/*
 * ADC pin implementation
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>

#include <lwiot/adcchip.h>
#include <lwiot/adcpin.h>

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
}
