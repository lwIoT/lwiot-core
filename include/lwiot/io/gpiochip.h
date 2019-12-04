/*
 * General port I/O interface class.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file gpiochip.h

#pragma once

#include <lwiot.h>

#include <lwiot/io/gpiopin.h>

#ifdef __cplusplus
#include <lwiot/stl/vector.h>

namespace lwiot
{
	/**
	 * @brief GPIO IRQ edge.
	 * @ingroup io
	 */
	enum IrqEdge {
		IrqNone = 0, //!< Disable the IRQ.
		IrqRising, //!< Trigger on the rising edge.
		IrqFalling, //!< Trigger on the falling edge.
		IrqRisingFalling //!< Trigger on both edges.
	};

	/**
	 * @brief GPIO peripheral abstractions.
	 * @ingroup io
	 */
	class GpioChip {
	public:
#ifdef CONFIG_PIN_VECTOR
		virtual ~GpioChip(); //!< GPIO chip destructor.
#else
		virtual ~GpioChip() = default;
#endif

		/**
		 * @brief Set a pins direction.
		 * @param pin Pin to set the direction for.
		 * @param mode Direction to set.
		 */
		virtual void mode(int pin, const PinMode& mode) = 0;

		/**
		 * @brief Write to \p pin.
		 * @param pin Pin to write to.
		 * @param value Value to write.
		 * @note \p pin must be OUTPUT or OUTPUT_OPEN_DRAIN.
		 */
		virtual void write(int pin, bool value) = 0;

		/**
		 * @brief Read from \p pin.
		 * @param pin Pin to read from.
		 * @return Value of \p pin.
		 */
		virtual bool read(int pin) const = 0;

		/**
		 * @brief Configure \p pin as open drain pin.
		 * @param pin Pin to configure.
		 */
		virtual void setOpenDrain(int pin) = 0;

		/**
		 * @brief Write to an open drain pin.
		 * @param pin Pin to write to.
		 * @param value Value to write.
		 * @note \p pin must be OUTPUT_OPEN_DRAIN.
		 */
		virtual void odWrite(int pin, bool value) = 0;

		/**
		 * @brief Attatch an IRQ handler to \p pin.
		 * @param pin Pin to attach an IRQ handler to.
		 * @param handler Handler to attach.
		 * @param edge Edge to trigger on.
		 */
		virtual void attachIrqHandler(int pin, irq_handler_t handler, IrqEdge edge) = 0;

		/**
		 * @brief Disable an IRQ.
		 * @param pin Pin to disable the IRQ for.
		 */
		virtual void detachIrqHandler(int pin) = 0;

		/**
		 * @brief Configure a pin as input.
		 * @param pin Pin to configure.
		 */
		virtual void input(int pin);

		/**
		 * @brief Configure a pin as output.
		 * @param pin Pin to configure.
		 */
		virtual void output(int pin);

		/**
		 * @brief Get the number of pins.
		 * @return Number of pins.
		 */
		const unsigned int& pins() const;

		/**
		 * @brief Shift in a byte of data one at a time.
		 * @param dpin Data pin.
		 * @param cpin Clock pin.
		 * @param lsb Bit order.
		 * @param count Number of bits.
		 * @param delay Delay in microseconds.
		 * @return Value read from \p dpin.
		 */
		virtual uint8_t shiftIn(int dpin, int cpin, bool lsb, uint8_t count, int delay);

		/**
		 * @brief Shift a binary value into a GPIO pin.
		 * @param dpin Data pin.
		 * @param cpin Clock pin.
		 * @param lsb Bit order.
		 * @param val Value to write.
		 * @param count Number of bits to write.
		 * @param delay Delay in microseconds.
		 * @return Error code
		 */
		virtual int shiftOut(int dpin, int cpin, bool lsb, uint8_t val, uint8_t count, int delay);

#ifdef CONFIG_PIN_VECTOR
		/**
		 * @brief ADC chip to pin conversion
		 * @param idx Pin number.
		 * @return GpioPin object for \p idx.
		 */
		virtual GpioPin& operator[] (const size_t& idx);
		/**
		 * @brief ADC chip to pin conversion
		 * @param idx Pin number.
		 * @return GpioPin object for \p idx.
		 */
		virtual GpioPin& pin(size_t idx);
#endif

	protected:
		/**
		 * @brief Construct a new GPIO chip.
		 * @param pins Number of pins.
		 */
		explicit GpioChip(int pins);

	private:
		/* attirbutes */
		unsigned int _nr;
#ifdef CONFIG_PIN_VECTOR
		stl::Vector<GpioPin*> _iopins;
		void buildPinVector();
#endif
	};
}

extern lwiot::GpioChip& gpio;
#endif
