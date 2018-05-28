/*
 * General port I/O interface class.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot.h>

#include <lwiot/gpiopin.h>
#include <lwiot/vector.h>

#if CXX
namespace lwiot
{
	enum IrqEdge {
		IrqNone = 0,
		IrqRising,
		IrqFalling,
		IrqRisingFalling
	};

	class GpioChip {
	public:
#ifdef CONFIG_PIN_VECTOR
		virtual ~GpioChip();
#else
		virtual ~GpioChip() = default;
#endif

		virtual void mode(int pin, const PinMode& mode) = 0;
		virtual void write(int pin, bool value) = 0;
		virtual bool read(int pin) = 0;

		virtual void setOpenDrain(int pin) = 0;
		virtual void odWrite(int pin, bool value) = 0;

		virtual void attachIrqHandler(int pin, irq_handler_t handler, IrqEdge edge) = 0;

		virtual void input(int pin);
		virtual void output(int pin);

		const unsigned int& pins() const;

		virtual uint8_t shiftIn(int dpin, int cpin, bool lsb, uint8_t count, int delay);
		virtual int shiftOut(int dpin, int cpin, bool lsb, uint8_t val, uint8_t count, int delay);

#ifdef CONFIG_PIN_VECTOR
		virtual GpioPin& operator[] (const size_t& idx);
		virtual GpioPin& pin(size_t idx);
#endif

	protected:
		explicit GpioChip(int pins);

	private:
		/* attirbutes */
		unsigned int _nr;
#ifdef CONFIG_PIN_VECTOR
		Vector<GpioPin*> _iopins;
		void buildPinVector();
#endif
	};
}

extern lwiot::GpioChip& gpio;
#endif
