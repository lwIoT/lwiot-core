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

	GpioPin::GpioPin() : GpioPin(-1)
	{
	}

	GpioPin::GpioPin(int pin, GpioChip& chip) : _pin(pin), _chip(chip)
	{
		this->_open_drain = false;
	}

	GpioPin::~GpioPin() = default;

	GpioPin& GpioPin::operator =(int num)
	{
		this->_pin = num;
		return *this;
	}

	GpioPin& GpioPin::operator =(const GpioPin& pin)
	{
		this->_pin = pin.pin();
		this->_chip = pin._chip;

		return *this;
	}

	GpioPin::operator int() const
	{
		return this->_pin;
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

	void RAM_ATTR GpioPin::write(bool value)
	{
		if(this->_open_drain)
			this->_chip.odWrite(this->_pin, value);
		else
			this->_chip.write(this->_pin, value);
	}

	void RAM_ATTR GpioPin::operator()(bool value)
	{
		if(this->_open_drain)
			this->_chip.odWrite(this->_pin, value);
		else
			this->_chip.write(this->_pin, value);
	}

	GpioPin& RAM_ATTR GpioPin::operator<<(bool value)
	{
		if(this->_open_drain)
			this->_chip.odWrite(this->_pin, value);
		else
			this->_chip.write(this->_pin, value);

		return *this;
	}

	bool RAM_ATTR GpioPin::read() const
	{
		return this->_chip.read(this->_pin);
	}

	GpioPin& RAM_ATTR GpioPin::operator>>(bool& value)
	{
		value = this->_chip.read(this->_pin);
		return *this;
	}

	bool RAM_ATTR GpioPin::operator()(void) const
	{
		return this->_chip.read(this->_pin);
	}

	RAM_ATTR GpioPin::operator bool() const
	{
		return this->_chip.read(this->_pin);
	}

	bool RAM_ATTR GpioPin::operator!() const
	{
		return !this->_chip.read(this->_pin);
	}

	int GpioPin::pin() const
	{
		return this->_pin;
	}

	bool GpioPin::operator ==(const GpioPin& pin)
	{
		return this->_pin == pin.pin();
	}

	bool GpioPin::operator <(const GpioPin& pin)
	{
		return this->_pin < pin.pin();
	}

	bool GpioPin::operator >(const GpioPin& pin)
	{
		return this->_pin > pin.pin();
	}

	bool GpioPin::operator <=(const GpioPin& pin)
	{
		return this->_pin <= pin.pin();
	}

	bool GpioPin::operator >=(const GpioPin& pin)
	{
		return this->_pin >= pin.pin();
	}
}
