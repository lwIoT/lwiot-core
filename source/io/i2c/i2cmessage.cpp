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
#include <lwiot/stl/forward.h>

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

	I2CMessage::I2CMessage(lwiot::I2CMessage &&other) noexcept : ByteBuffer(stl::forward<I2CMessage>(other)),
		_addr(other.address()), _is_10bit(other.is10Bit()),
		_readop(other.isRead()), _repstart(other._repstart)
	{
	}

	I2CMessage& I2CMessage::operator=(lwiot::I2CMessage &&msg) noexcept
	{
		ByteBuffer::move(msg);

		this->_addr = msg.address();
		this->_is_10bit = msg.is10Bit();
		this->_readop = msg.isRead();
		this->_repstart = msg.repstart();

		return *this;
	}

	I2CMessage& I2CMessage::operator=(const lwiot::I2CMessage &msg)
	{
		ByteBuffer::copy(msg);

		this->_addr = msg.address();
		this->_is_10bit = msg.is10Bit();
		this->_readop = msg.isRead();
		this->_repstart = msg.repstart();

		return *this;
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

	void I2CMessage::markAsReadOperation(bool read)
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

	void I2CMessage::setAddress(uint16_t addr, bool is10bit)
	{
		this->_addr = addr;
		this->_is_10bit = is10bit;
	}
}
