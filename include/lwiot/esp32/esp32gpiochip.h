/*
 * GPIO chip for the ESP32 chip.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/lwiot.h>
#include <lwiot/gpiochip.h>

#ifdef __cplusplus
namespace lwiot
{
	class Esp32GpioChip : public GpioChip {
	public:
		explicit Esp32GpioChip();
		virtual ~Esp32GpioChip() = default;

		virtual void mode(int pin, const PinMode& mode) override;
		virtual void write(int pin, bool value) override;
		virtual bool read(int pin) const override;

		virtual void setOpenDrain(int pin) override;
		virtual void odWrite(int pin, bool value) override;
		virtual void attachIrqHandler(int pin, irq_handler_t handler, IrqEdge edge) override;

	private:
		int mapIrqType(const IrqEdge& edge) const;
	};
}
#endif

#define RAW_INPUT 1
#define RAW_OUTPUT 2 
#define RAW_OUTPUT_OPENDRAIN 3

#ifdef __cplusplus
extern "C" void pinMode(int pin, int mode);
extern "C" void digitalWrite(int pin, bool value);
#else
extern void pinMode(int pin, int mode);
extern void digitalWrite(int pin, bool value);
#endif
