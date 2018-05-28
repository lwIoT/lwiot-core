/*
 * GPIO pin wrapper.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot.h>

#ifdef CXX
namespace lwiot
{
	class GpioChip;

	enum PinMode {
		INPUT,
		INPUT_PULLUP,
		INPUT_NOPULLUP,
		OUTPUT,
		OUTPUT_OPEN_DRAIN
	};

	class GpioPin {
	public:
		GpioPin(int pin);
		explicit GpioPin(int pin, GpioChip& gpio);
		virtual ~GpioPin();

		void input();
		void output();
		void setOpenDrain();

		void write(bool high);
		bool read();
		void mode(const PinMode& mode);

		uint8_t shiftIn(const GpioPin& clock, bool lsb, uint8_t count, int delay);
		int shiftOut(const GpioPin& clock, bool lsb, uint8_t value, uint8_t count, int delay);

		int pin() const;

	private:
		bool _open_drain;
		int _pin;
		GpioChip& _chip;
	};
}
#endif
