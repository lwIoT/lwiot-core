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
		this->_data = (uint8_t*)lwiot_mem_alloc(size);
		memset(this->_data, 0, size);
	}

	ByteBuffer::ByteBuffer(const ByteBuffer& other) : ByteBuffer(other.count())
	{
		memcpy(this->_data, other._data, other.count());
		this->_count = other.count();
		this->_index = other._index;
	}


	ByteBuffer::~ByteBuffer()
	{
		lwiot_mem_free(this->_data);
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

	bool ByteBuffer::operator ==(const ByteBuffer& rhs) const
	{
		if(this->count() != rhs.count())
			return false;

		if(this->_index != rhs._index)
			return false;

		return memcmp(this->_data, rhs._data, this->count()) == 0;
	}

	void ByteBuffer::grow(size_t size)
	{
		uint8_t *old = this->_data;

		this->_data = (uint8_t*)lwiot_mem_zalloc(size + this->count());
		Countable::grow(size);
		memset(this->_data, 0, this->count());

		memcpy(this->_data, old, this->_index);
		lwiot_mem_free(old);
	}
}
