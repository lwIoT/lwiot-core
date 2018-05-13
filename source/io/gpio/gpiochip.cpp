/*
 * General port I/O interface class.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <lwiot/gpiochip.h>

namespace lwiot
{
	GpioChip::GpioChip(int pins)
	{
		this->_nr = pins;
	}

	const unsigned int& GpioChip::pins() const
	{
		return this->_nr;
	}

	void GpioChip::input(int pin)
	{
		this->mode(pin, INPUT);
	}

	void GpioChip::output(int pin)
	{
		this->mode(pin, OUTPUT);
	}
}
