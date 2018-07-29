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
		explicit I2CMessage(const size_t& size);
		explicit I2CMessage(const I2CMessage& other);
		virtual ~I2CMessage() = default;

		uint16_t address() const;
		bool isReadOnly() const;
		bool is10Bit() const;
		size_t available() const;

		void setAddress(uint16_t addr, bool is10bit, bool readop);
		void markAsRead(bool read);

		using ByteBuffer::operator[];

	private:
		uint16_t _addr;
		uint16_t _is_10bit;
		bool _readop;
	};
}
