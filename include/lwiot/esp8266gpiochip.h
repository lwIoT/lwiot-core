/*
 * GPIO strategy for the ESP8266.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot.h>

#include <lwiot/gpiochip.h>

#ifdef PINS
#undef PINS
#endif

#define PINS 11

#ifdef CXX
namespace lwiot
{
	class Esp8266GpioChip : public GpioChip {
	public:
		explicit Esp8266GpioChip();
		virtual ~Esp8266GpioChip() = default;

		virtual void mode(int pin, const PinMode& mode) override;
		virtual void write(int pin, bool value) override;
		virtual bool read(int pin) override;

		virtual void setOpenDrain(int pin) override;
		virtual void odWrite(int pin, bool value) override;
		virtual void attachIrqHandler(int pin, irq_handler_t handler, IrqEdge edge) override;

	private:
		void mode16(const PinMode& mode);
		void setPullup(int pin, bool enabled);
	};
}
#endif
