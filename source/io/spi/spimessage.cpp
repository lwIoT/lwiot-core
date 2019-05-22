/*
 * SPI message implementation.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/log.h>
#include <lwiot/stream.h>
#include <lwiot/types.h>
#include <lwiot/bufferedstream.h>
#include <lwiot/io/spimessage.h>

namespace lwiot
{
	SpiMessage::SpiMessage(const size_t& size, int pin) : SpiMessage(size, GpioPin(pin))
	{
	}

	SpiMessage::SpiMessage(const size_t &size, const lwiot::GpioPin &pin) :
		_tx(size), _rx(size), _idx(0), _size(size), _cspin(pin)
	{
	}

	ByteBuffer& SpiMessage::txdata() 
	{
		return this->_tx;
	}

	ByteBuffer& SpiMessage::rxdata()
	{
		return this->_rx;
	}

	SpiMessage& SpiMessage::operator<<(uint8_t byte)
	{
		this->_tx.write(byte);
		return *this;
	}

	const GpioPin& SpiMessage::cspin() const
	{
		return this->_cspin;
	}

	SpiMessage& SpiMessage::operator<<(const uint8_t *msg)
	{
		while(*msg) {
			this->_tx.write(*msg);
			msg++;
		}

		return *this;
	}

	SpiMessage& SpiMessage::operator>>(uint8_t& byte)
	{
		byte = this->_rx[this->_idx];
		this->_idx++;

		return *this;
	}

	const size_t& SpiMessage::size() const
	{
		return this->_size;
	}
}
