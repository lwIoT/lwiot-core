/*
 * General port I/O interface class.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdio.h>

#include <lwiot/gpiochip.h>
#include <lwiot/gpiopin.h>
#include <lwiot/error.h>

extern lwiot::GpioPin iopins[];

namespace lwiot
{
#ifdef CONFIG_PIN_VECTOR
	GpioChip::GpioChip(int pins) : _iopins(pins + 1)
#else
	GpioChip::GpioChip(int pins)
#endif
	{
		this->_nr = pins;
#ifdef CONFIG_PIN_VECTOR
		this->buildPinVector();
#endif
	}

#ifdef CONFIG_PIN_VECTOR
	GpioChip::~GpioChip()
	{
		for(auto pin : this->_iopins) {
			delete pin;
		}
	}

	void GpioChip::buildPinVector()
	{
		for(unsigned int i = 0; i < this->_nr; i++) {
			auto pin = new GpioPin(i, *this);
			this->_iopins.add(pin);
		}
	}

	GpioPin& GpioChip::operator[] (const size_t& pin)
	{
		auto pinObj = this->_iopins[pin];
		return *pinObj;
	}

	GpioPin& GpioChip::pin(size_t pin)
	{
		return *this->_iopins.get(pin);
	}
#endif

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

	uint8_t GpioChip::shiftIn(int dpin, int cpin, bool lsb, uint8_t count, int delay)
	{
		uint8_t value;

		if(count > 8)
			return 0;

		value = 0;

		for(int idx = 0; idx < count; idx++) {
			this->write(cpin, true);
			udelay(delay);

			if(lsb)
				value |= this->read(dpin);
			else
				value |= this->read(dpin << ((count - 1) - idx));

			this->write(cpin, false);
			udelay(delay);
		}

		return value;
	}

	int GpioChip::shiftOut(int dpin, int cpin, bool lsb, uint8_t val, uint8_t count, int delay)
	{
		for(auto idx = 0; idx < count; idx++) {
			if(lsb)
				this->write(dpin, !!(val & (1 << idx)));
			else
				this->write( dpin, !!(val & (1 << ((count - 1) - idx))) );

			this->write(cpin, true);
			udelay(delay);
			this->write(cpin, false);
			udelay(delay);
		}

		return -EOK;
	}
}
