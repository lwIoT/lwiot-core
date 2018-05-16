/*
 * General port I/O interface class.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot.h>

#include <lwiot/gpiopin.h>

#if CXX
namespace lwiot
{
	class GpioChip {
	public:
		virtual ~GpioChip() = default;

		virtual void mode(int pin, const PinMode& mode) = 0;
		virtual void write(int pin, bool value) = 0;
		virtual bool read(int pin) = 0;

		virtual void setOpenDrain(int pin) = 0;
		virtual void odWrite(int pin, bool value) = 0;

		virtual void input(int pin);
		virtual void output(int pin);

		const unsigned int& pins() const;

	protected:
		explicit GpioChip(int pins);

	private:
		unsigned int _nr;
	};
}

extern lwiot::GpioChip& gpio;
#endif
