/*
 * GPIO pin wrapper.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <lwiot/gpiopin.h>
#include <lwiot/gpiochip.h>

namespace lwiot
{
	GpioPin::GpioPin(int pin) : _pin(pin), _chip(gpio)
	{ }

	GpioPin::GpioPin(int pin, GpioChip& chip) : _pin(pin), _chip(chip)
	{ }

	GpioPin::~GpioPin()
	{
		this->input();
		this->write(false);
	}

	void GpioPin::input()
	{
		this->mode(INPUT);
	}

	void GpioPin::output()
	{
		this->mode(OUTPUT);
	}

	void GpioPin::setOpenDrain()
	{
		this->mode(OUTPUT_OPEN_DRAIN);
		this->write(true);
	}

	void GpioPin::mode(const PinMode& mode)
	{
		this->_chip.mode(this->_pin, mode);
	}

	void GpioPin::write(bool value)
	{
		this->_chip.write(this->_pin, value);
	}

	bool GpioPin::read()
	{
		return this->_chip.read(this->_pin);
	}

	int GpioPin::pin()
	{
		return this->_pin;
	}
}
