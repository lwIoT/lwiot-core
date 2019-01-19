/*
 * GPIO strategy for the ESP8266.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <esp8266.h>
#include <lwiot.h>
#include <lwiot/io/gpiochip.h>

#include <esp/gpio.h>

#ifdef PINS
#undef PINS
#endif

#define PINS 16

#ifdef CXX
namespace lwiot { namespace esp8266
{
	class GpioChip : public lwiot::GpioChip {
	public:
		explicit GpioChip();
		virtual ~GpioChip() = default;

		virtual void mode(int pin, const PinMode& mode) override;
		virtual void write(int pin, bool value) override;
		virtual bool read(int pin) const override;

		virtual void setOpenDrain(int pin) override;
		virtual void odWrite(int pin, bool value) override;
		virtual void attachIrqHandler(int pin, irq_handler_t handler, IrqEdge edge) override;

	private:
		void mode16(const PinMode& mode);
		void setPullup(int pin, bool enabled);
		gpio_inttype_t mapIrqEdge(const IrqEdge& edge) const;
	};
}
}
#endif
