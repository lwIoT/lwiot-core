/*
 * Abstract SPI bus definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file spibus.h

#pragma once

#include <lwiot.h>
#include <stdlib.h>

#include <lwiot/io/spimessage.h>
#include <lwiot/types.h>
#include <lwiot/io/gpiopin.h>
#include <lwiot/stl/vector.h>

#ifdef __cplusplus
namespace lwiot
{
	/**
	 * @brief SPI bus abstraction.
	 * @ingroup io
	 */
	class SpiBus {
	public:
		/**
		 * @brief Create a new SPI bus.
		 * @param mosi MOSI pin.
		 * @param miso MISO pin.
		 * @param clk Clock pin.
		 * @param freq Bus frequency.
		 */
		explicit SpiBus(int mosi, int miso, int clk, uint32_t freq = 1000000UL);
		virtual ~SpiBus() = default; //!< SPI bus destructor.

		/**
		 * @brief Getter for the bus frequency.
		 * @return The bus frequency.
		 */
		const uint32_t& frequency() const;

		/**
		 * @brief Getter for the MOSI pin.
		 * @return An IO pin object.
		 */
		const GpioPin& miso() const;

		/**
		 * @brief Getter for the MISO pin.
		 * @return An IO pin object.
		 */
		const GpioPin& mosi() const;

		/**
		 * @brief Getter for the CLK pin.
		 * @return An IO pin object.
		 */
		const GpioPin& clk() const;

		/**
		 * @brief Transfer a SPI message.
		 * @param msg Message to transfer.
		 * @return A success indicator.
		 */
		virtual bool transfer(SpiMessage& msg) = 0;

		/**
		 * @brief Transfer a SPI message.
		 * @param msgs Messages to transfer.
		 * @return A success indicator.
		 */
		virtual bool transfer(stl::Vector<SpiMessage>& msgs);

		/**
		 * @brief Set the bus frequency.
		 * @param freq Frequency to set.
		 */
		virtual void setFrequency(uint32_t freq);

	protected:
		GpioPin _miso; //!< MISO pin.
		GpioPin _mosi; //!< MOSI pin.
		GpioPin _clk; //!< Clock pin.

	private:
		uint32_t _freq;
	};
}
#endif
