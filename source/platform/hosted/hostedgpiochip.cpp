/*
 * Hosted (Windows / Unix) GPIO chip.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/log.h>
#include <lwiot/io/gpiochip.h>
#include <lwiot/util/pair.h>
#include <lwiot/hosted/hostedgpiochip.h>

#include <vector>
#include <map>

namespace lwiot
{
	HostedGpioChip::HostedGpioChip(int pins ) noexcept : GpioChip(pins)
	{ }

	void HostedGpioChip::write(int pin, bool value)
	{
		print_dbg("Writing %u to %i\n", value, pin);
	}

	void HostedGpioChip::odWrite(int pin, bool value)
	{
		this->write(pin, value);
	}

	bool HostedGpioChip::read(int pin) const
	{
		return false;
	}

	void HostedGpioChip::mode(int pin, const PinMode& mode)
	{
		print_dbg("Setting mode %u on pin %i\n", mode, pin);
	}

	void HostedGpioChip::setOpenDrain(int pin)
	{
		this->mode(pin, OUTPUT_OPEN_DRAIN);
	}

	void HostedGpioChip::attachIrqHandler(int pin, irq_handler_t handler, IrqEdge edge)
	{
	}
}

static lwiot::HostedGpioChip hosted_gpio(10);
lwiot::GpioChip& gpio = hosted_gpio;
