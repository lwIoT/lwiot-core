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
	GpioPin::GpioPin(int pin) : GpioPin(pin, gpio)
	{
	}

	GpioPin::GpioPin(int pin, GpioChip& chip) : _pin(pin), _chip(chip)
	{
		this->_open_drain = false;
	}

	GpioPin::~GpioPin()
	{
		this->input();
		this->write(false);
	}

	GpioPin& GpioPin::operator =(int num)
	{
		this->_pin = num;
		return *this;
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

	uint8_t GpioPin::shiftIn(const GpioPin& clk, bool lsb, uint8_t count, int delay)
	{
		return this->_chip.shiftIn(this->_pin, clk.pin(), lsb, count, delay);
	}

	int GpioPin::shiftOut(const GpioPin& clk, bool lsb, uint8_t value, uint8_t count, int delay)
	{
		return this->_chip.shiftOut(this->_pin, clk.pin(), lsb, value, count, delay);
	}

	void GpioPin::mode(const PinMode& mode)
	{
		if(mode == OUTPUT_OPEN_DRAIN)
			this->_open_drain = true;
		else
			this->_open_drain = false;

		this->_chip.mode(this->_pin, mode);
	}

	void GpioPin::write(bool value)
	{
		if(this->_open_drain)
			this->_chip.odWrite(this->_pin, value);
		else
			this->_chip.write(this->_pin, value);
	}

	bool GpioPin::read()
	{
		return this->_chip.read(this->_pin);
	}

	int GpioPin::pin() const
	{
		return this->_pin;
	}
}
