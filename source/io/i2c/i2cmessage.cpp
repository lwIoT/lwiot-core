/*
 * I2C transmission data container.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/log.h>
#include <lwiot/error.h>
#include <lwiot/types.h>
#include <lwiot/io/i2cmessage.h>

#define I2CMESSAGE_DEFAULT_SIZE 8UL

namespace lwiot
{
	I2CMessage::I2CMessage() : I2CMessage(I2CMESSAGE_DEFAULT_SIZE)
	{
	}

	I2CMessage::I2CMessage(uint16_t addr, bool is10bit, const size_t& size) : ByteBuffer(size, true),
		_addr(addr), _is_10bit(is10bit), _readop(false), _repstart(false)
	{
	}

	I2CMessage::I2CMessage(const size_t& size) : I2CMessage(0, false, size)
	{
	}

	I2CMessage::I2CMessage(const I2CMessage& other) : ByteBuffer(other),
		_addr(other.address()), _is_10bit(other.is10Bit()),
		_readop(other.isRead()), _repstart(other._repstart)
	{
	}

	I2CMessage::I2CMessage(const lwiot::ByteBuffer &buffer) :
		ByteBuffer(buffer), _addr(0), _is_10bit(false), _readop(false), _repstart(false)
	{
	}

	uint16_t I2CMessage::address() const
	{
		return this->_addr;
	}

	bool I2CMessage::isRead() const
	{
		return this->_readop;
	}

	bool I2CMessage::is10Bit() const
	{
		return this->_is_10bit;
	}

	size_t I2CMessage::available() const
	{
		auto size = this->count();

		size -= this->_index;
		return size;
	}

	void I2CMessage::setAddress(uint16_t addr, bool is10bit, bool readop)
	{
		this->_addr = addr;
		this->_is_10bit = is10bit;
		this->_readop = readop;
	}

	void I2CMessage::markAsRead(bool read)
	{
		this->_readop = read;
	}

	void I2CMessage::setRepeatedStart(bool repstart)
	{
		this->_repstart = repstart;
	}

	bool I2CMessage::repstart() const
	{
		return this->_repstart;
	}

	const size_t& I2CMessage::length() const
	{
		return this->_index;
	}
}
