/*
 * GPIO pin wrapper.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

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

		int pin();

	private:
		int _pin;
		GpioChip& _chip;
	};
}
