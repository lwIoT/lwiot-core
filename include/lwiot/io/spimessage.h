/*
 * SPI message definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file spimessage.h

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/log.h>
#include <lwiot/stream.h>
#include <lwiot/types.h>
#include <lwiot/bytebuffer.h>

#include <lwiot/io/gpiopin.h>

#ifdef __cplusplus
namespace lwiot
{
	/**
	 * @brief SPI message class.
	 * @ingroup io
	 */
	class SpiMessage {
	public:
		/**
		 * @brief Construct a new SpiMessage object.
		 * @param size Message size.
		 * @param cs Chip select pin.
		 */
		explicit SpiMessage(const size_t& size, int cs = 0);

		/**
		 * @brief Construct a new SpiMessage object.
		 * @param size Message size.
		 * @param pin Chip select pin.
		 */
		explicit SpiMessage(const size_t& size, const GpioPin& pin);

		/**
		 * @brief SPI message destructor.
		 */
		virtual ~SpiMessage() = default;

		/**
		 * @brief Get the receive buffer.
		 * @return The receive buffer.
		 */
		ByteBuffer& rxdata();

		/**
		 * @brief Get the transmit buffer.
		 * @return The transmit buffer.
		 */
		ByteBuffer& txdata();

		/**
		 * @brief Shift data into the message.
		 * @param byte Data to add to the transmit buffer.
		 * @return A reference to \p *this.
		 */
		SpiMessage& operator <<(uint8_t byte);

		/**
		 * @brief Shift data into the message.
		 * @param msg Data to add to the transmit buffer.
		 * @note \p msg must contain a null terminator.
		 * @return A reference to \p *this.
		 */
		SpiMessage& operator<<(const uint8_t *msg);

		/**
		 * @brief Shift data out of the receive buffer.
		 * @param byte Buffer object.
		 * @return A reference to \p *this.
		 */
		SpiMessage& operator >>(uint8_t& byte);

		/**
		 * @brief Get the message size.
		 * @return The message size.
		 */
		const size_t& size() const;

		/**
		 * @brief Get the chip select pin.
		 * @return The chip select pin.
		 * @see GpioPin
		 */
		const GpioPin& cspin() const;

	private:
		ByteBuffer _tx;
		ByteBuffer _rx;
		size_t _idx;
		size_t _size;
		GpioPin _cspin;
	};
}
#endif
