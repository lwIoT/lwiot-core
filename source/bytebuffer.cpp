/*
 * Byte array wrapper class.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/log.h>
#include <lwiot/bytebuffer.h>

namespace lwiot
{
	ByteBuffer::ByteBuffer() : ByteBuffer(BYTEBUFFER_DEFAULT_SIZE)
	{
	}

	ByteBuffer::ByteBuffer(const size_t& size) : Countable(size), _index(0)
	{
		this->_data = new uint8_t[size];
		memset(this->_data, 0, size);
	}

	ByteBuffer::~ByteBuffer()
	{
		delete[] this->_data;
	}

	uint8_t& ByteBuffer::operator[](const size_t& idx)
	{
		return this->_data[idx];
	}

	const uint8_t& ByteBuffer::operator[](const size_t& idx) const
	{
		return this->_data[idx];
	}

	const uint8_t* ByteBuffer::begin() const
	{
		return this->_data;
	}

	const uint8_t* ByteBuffer::end() const
	{
		return this->_data + this->count();
	}

	void ByteBuffer::write(uint8_t byte)
	{
		this->write(&byte, sizeof(byte));
	}

	void ByteBuffer::write(const uint8_t* bytes, size_t num)
	{
		if(this->_index + num >= this->count())
			this->grow(num + 10);

		if(num == 1) {
			this->_data[this->_index++] = *bytes;
			return;
		}

		memcpy(&this->_data[this->_index], bytes, num);
		this->_index += num;
	}

	void ByteBuffer::grow(size_t size)
	{
		const uint8_t *old = this->_data;

		this->_data = new uint8_t[size + this->count()];
		Countable::grow(size);
		memset(this->_data, 0, this->count());

		memcpy(this->_data, old, this->_index);
		delete[] old;
	}
}
