/*
 * GPIO chip for the ESP32 chip.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/lwiot.h>
#include <lwiot/io/gpiochip.h>

#ifdef __cplusplus
namespace lwiot { namespace esp32
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
		int mapIrqType(const IrqEdge& edge) const;
	};
}
}
#endif
