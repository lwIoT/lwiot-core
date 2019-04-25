/*
 * Digital to analog conversion pin
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <lwiot.h>
#include <stdlib.h>

#include <lwiot/io/dacchip.h>
#include <lwiot/io/dacpin.h>

namespace lwiot
{
	DacPin::DacPin(int num) : DacPin(num, dac)
	{ }

	DacPin::DacPin(int num, DacChip& chip) : _chip(chip), _pin(num)
	{ }

	void DacPin::write(const size_t& volts)
	{
		this->_chip.write(this->_pin, volts);
	}
}
