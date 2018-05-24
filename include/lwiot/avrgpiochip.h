/*
 * AVR ATmega GPIO implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/gpiochip.h>

#ifdef ARDUINO_MEGA
#define PINS 54
#elif defined(ARDUINO_UNO)
#define PINS 13
#endif

#define NOT_A_PIN 0
#define NOT_A_PORT 0

#ifdef __cplusplus
namespace lwiot
{
	class AvrGpioChip : public GpioChip {
	public:
		explicit AvrGpioChip();
		virtual ~AvrGpioChip() = default;

		virtual void mode(int pin, const PinMode& mode) override;
		virtual void write(int pin, bool value) override;
		virtual bool read(int pin) override;

		virtual void setOpenDrain(int pin) override;
		virtual void odWrite(int pin, bool value) override;
		virtual void attachIrqHandler(int pin, irq_handler_t handler, IrqEdge edge) override;
	};
}
#endif
