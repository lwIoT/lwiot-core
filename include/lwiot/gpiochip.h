/*
 * General port I/O interface class.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot.h>

#if CXX
namespace lwiot
{
	enum PinMode {
		INPUT,
		INPUT_PULLUP,
		INPUT_NOPULLUP,
		OUTPUT,
		OUTPUT_OPEN_DRAIN
	};

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
#endif
