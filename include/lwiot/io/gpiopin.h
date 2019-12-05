/*
 * GPIO pin wrapper.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file gpiopin.h

#pragma once

#include <lwiot.h>

#ifdef __cplusplus
namespace lwiot
{
	class GpioChip;

	/**
	 * @brief GPIO pinmode.
	 * @ingroup io
	 */
	enum PinMode {
		INPUT, //!< Input pin.
		INPUT_PULLUP, //!< Pinput pin, pull-ups enabled.
		INPUT_NOPULLUP, //!< Pinput pin, pull-ups disabled.
		OUTPUT, //!< Output pin.
		OUTPUT_OPEN_DRAIN //!< Output, configured as open drain.
	};

	/**
	 * @brief GPIO pin abstraction.
	 * @ingroup io
	 */
	class GpioPin {
	public:
		/**
		 * @brief Construct a GpioPin.
		 * @param pin Pin number.
		 */
		GpioPin(int pin);

		/**
		 * @brief Construct a GpioPin.
		 */
		explicit GpioPin();
		/**
		 * @brief Construct a GpioPin.
		 * @param pin Pin number.
		 * @param gpio GPIO peripheral.
		 */
		explicit GpioPin(int pin, GpioChip& gpio);

		virtual ~GpioPin(); //!< GpioPin destructor.

		/**
		 * @brief Assignment operator.
		 * @param pin Pin number.
		 * @return A reference to \p *this.
		 */
		GpioPin& operator =(int pin);
		/**
		 * @brief Copy assignment operator.
		 * @param pin GpioPin to copy.
		 * @return A reference to \p *this.
		 */
		GpioPin& operator =(const GpioPin& pin);

		/**
		 * @brief Write a value.
		 * @param value Value to write.
		 */
		void write(bool value);

		/**
		 * @brief Write a value.
		 * @param value Value to write.
		 * @return A reference to \p *this.
		 */
		GpioPin& operator <<(bool value);

		/**
		 * @brief Write a value.
		 * @param value Value to write.
		 * @see write
		 */
		void operator ()(bool value);

		/**
		 * @brief Read from a GpioPin.
		 * @return The value read.
		 */
		bool read() const;

		/**
		 * @brief Read a value.
		 * @param value Value read.
		 * @return A reference to \p *this.
		 */
		GpioPin& operator >>(bool& value);
		/**
		 * @brief Read from a GpioPin.
		 * @return The value read.
		 * @see read
		 */
		bool operator ()() const;
		/**
		 * @brief Read from a GpioPin.
		 * @return The value read.
		 * @see read
		 */
		explicit operator bool() const;

		/**
		 * @brief Read from a GpioPin.
		 * @return The value read.
		 */
		bool operator !() const;

		/**
		 * @brief Configure a pin as input.
		 */
		void input();
		/**
		 * @brief Configure a pin as output.
		 */
		void output();
		/**
		 * @brief Configure a pin as open drain.
		 */
		void setOpenDrain();

		/**
		 * @brief Configure the pin direction.
		 * @param mode Direction to set.
		 */
		void mode(const PinMode& mode);

		/**
		 * @brief Shift in a byte of data one at a time.
		 * @param clock Clock pin.
		 * @param lsb Bit order.
		 * @param count Number of bits.
		 * @param delay Delay in microseconds.
		 * @return Value read from \p dpin.
		 */
		uint8_t shiftIn(const GpioPin& clock, bool lsb, uint8_t count, int delay);

		/**
		 * @brief Shift a binary value into a GPIO pin.
		 * @param clock Clock pin.
		 * @param lsb Bit order.
		 * @param value Value to write.
		 * @param count Number of bits to write.
		 * @param delay Delay in microseconds.
		 * @return Error code
		 */
		int shiftOut(const GpioPin& clock, bool lsb, uint8_t value, uint8_t count, int delay);

		/**
		 * @brief Get the pin number.
		 * @return The pin number.
		 */

		int pin() const;
		/**
		 * @brief Get the pin number.
		 * @return The pin number.
		 */
		operator int() const;

		bool operator ==(const GpioPin& pin); //!< Comparison operator.
		bool operator >(const GpioPin& pin); //!< Comparison operator.
		bool operator <(const GpioPin& pin); //!< Comparison operator.
		bool operator >=(const GpioPin& pin); //!< Comparison operator.
		bool operator <=(const GpioPin& pin); //!< Comparison operator.

	private:
		bool _open_drain;
		int _pin;
		GpioChip& _chip;
	};
}
#endif
