/*
 * I2C transmission data container.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file i2cmessage.h

#pragma once

#include <lwiot/lwiot.h>
#include <lwiot/bytebuffer.h>
#include <lwiot/log.h>
#include <lwiot/error.h>
#include <lwiot/types.h>

namespace lwiot
{
	/**
	 * @brief I2C message.
	 * @ingroup io
	 */
	class I2CMessage : public ByteBuffer {
	public:
		/**
		 * @brief Construct an I2C message.
		 */
		explicit I2CMessage();
		/**
		 * @brief Construct an I2C message.
		 * @param addr  Target address.
		 * @param is10bit 10-bit address indicator.
		 * @param size Message size.
		 */
		explicit I2CMessage(uint16_t addr, bool is10bit, const size_t& size = 8);

		/**
		 * @brief Construct an I2C message.
		 * @param buffer Buffer to copy.
		 */
		explicit I2CMessage(const ByteBuffer& buffer);

		/**
		 * @brief Construct an I2C message.
		 * @param size Message size.
		 */
		explicit I2CMessage(const size_t& size);

		/**
		 * @brief Construct an I2C message.
		 * @param other  Message to copy.
		 */
		I2CMessage(const I2CMessage& other);

		/**
		 * @brief Construct an I2C message.
		 * @param other Message to move.
		 */
		I2CMessage(I2CMessage&& other) noexcept ;
		~I2CMessage() override = default; //!< Message destructor.

		/**
		 * @brief Assignment operator.
		 * @param msg Message to copy.
		 * @return A reference to \p *this.
		 */
		I2CMessage& operator=(const I2CMessage& msg);

		/**
		 * @brief Assignment operator.
		 * @param msg Message to move.
		 * @return A reference to \p *this.
		 */
		I2CMessage& operator=(I2CMessage&& msg) noexcept ;

		/**
		 * @brief Address getter.
		 * @return The message address.
		 */
		uint16_t address() const;

		/**
		 * @brief Read flag getter.
		 * @return Read/write indicator.
		 */
		bool isRead() const;

		/**
		 * @brief Address type getter.
		 * @return Address type indicator.
		 */
		bool is10Bit() const;

		/**
		 * @brief Check if data is available.
		 * @return Number of bytes of data available.
		 */
		size_t available() const;

		/**
		 * @brief Message length.
		 * @return
		 */
		const size_t& length() const;

		/**
		 * @brief Set the message address.
		 * @param addr Address to set.
		 * @param is10bit 10-bit address indicator.
		 * @param readop Read operation indicator.
		 */
		void setAddress(uint16_t addr, bool is10bit, bool readop);

		/**
		 * @brief Set the message address.
		 * @param addr Address to set.
		 * @param is10bit 10-bit address indicator.
		 */
		void setAddress(uint16_t addr, bool is10bit);

		/**
		 * @brief Set the message type.
		 * @param read Read indicator.
		 */
		void markAsReadOperation(bool read);

		/**
		 * @brief Set repeated start condition.
		 * @param repstart Repeated start indicator.
		 */
		void setRepeatedStart(bool repstart);

		/**
		 * @brief Check if \p *this needs a repeated start.
		 * @return Repeated start indicator.
		 */
		bool repstart() const;

		using ByteBuffer::operator[];

	private:
		uint16_t _addr;
		uint16_t _is_10bit;
		bool _readop;
		bool _repstart;
	};
}
