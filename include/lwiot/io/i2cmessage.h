/*
 * I2C transmission data container.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/lwiot.h>
#include <lwiot/bytebuffer.h>
#include <lwiot/log.h>
#include <lwiot/error.h>
#include <lwiot/types.h>

namespace lwiot
{
	class I2CMessage : public ByteBuffer {
	public:
		explicit I2CMessage();
		explicit I2CMessage(uint16_t addr, bool is10bit, const size_t& size = 8);
		explicit I2CMessage(const ByteBuffer& buffer);
		explicit I2CMessage(const size_t& size);
		I2CMessage(const I2CMessage& other);
		I2CMessage(I2CMessage&& other) noexcept ;
		~I2CMessage() override = default;

		I2CMessage& operator=(const I2CMessage& msg);
		I2CMessage& operator=(I2CMessage&& msg) noexcept ;


		uint16_t address() const;
		bool isRead() const;
		bool is10Bit() const;
		size_t available() const;
		const size_t& length() const;

		void setAddress(uint16_t addr, bool is10bit, bool readop);
		void setAddress(uint16_t addr, bool is10bit);
		void markAsReadOperation(bool read);

		void setRepeatedStart(bool repstart);
		bool repstart() const;

		using ByteBuffer::operator[];

	private:
		uint16_t _addr;
		uint16_t _is_10bit;
		bool _readop;
		bool _repstart;
	};
}
