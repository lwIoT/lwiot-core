/*
 * GPIO chip for the ESP32 chip.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/lwiot.h>
#include <lwiot/gpiochip.h>

#ifdef CXX
namespace lwiot
{
	class Esp32GpioChip : public GpioChip {
	public:
		explicit Esp32GpioChip();
		virtual ~Esp32GpioChip() = default;

		virtual void mode(int pin, const PinMode& mode) override;
		virtual void write(int pin, bool value) override;
		virtual bool read(int pin) override;

		virtual void setOpenDrain(int pin) override;
		virtual void odWrite(int pin, bool value) override;
		virtual void attachIrqHandler(int pin, irq_handler_t handler, IrqEdge edge) override;

	private:
		int mapIrqType(const IrqEdge& edge) const;
	};
}
#endif
