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

	ByteBuffer::ByteBuffer(const size_t& size, bool exactfit) : Countable(size), _index(0), _exactfit(exactfit)
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

#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__) || defined(WIN32)
	ByteBuffer::ByteBuffer(ByteBuffer&& other) noexcept :
		Countable(other.count()), _index(0), _data(nullptr), _exactfit(other._exactfit)
	{
		this->move(other);
	}
#endif

	ByteBuffer::~ByteBuffer()
	{
		lwiot_mem_free(this->_data);
	}

#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__) || defined(WIN32)
	ByteBuffer& ByteBuffer::operator=(ByteBuffer&& other) noexcept
	{
		this->move(other);
		return *this;
	}

	void ByteBuffer::move(ByteBuffer& other)
	{
		if(this->_data) {
			if(other._data && this->_count >= other._count) {
				memcpy(reinterpret_cast<void*>(this->_data),
				       reinterpret_cast<void*>(other._data),
					   other.count());
				this->_count = other.count();
				this->_index = other._index;

				other._count = 0;
				other._index = 0;
				return;
			} else {
				lwiot_mem_free(this->_data);
			}
		}

		this->_data = other._data;
		this->_index = other._index;
		this->_count = other._count;

		other._count = other._index = 0;
		other._data = reinterpret_cast<uint8_t*>(lwiot_mem_alloc(BYTEBUFFER_DEFAULT_SIZE));
	}
#endif

	ByteBuffer& ByteBuffer::operator=(const ByteBuffer& other)
	{
		auto size = other.count();

		lwiot_mem_free(this->_data);
		this->_data = static_cast<uint8_t*>(lwiot_mem_zalloc(size));

		memcpy(this->_data, other._data, size);
		this->_count = other.count();
		this->_index = other._index;

		return *this;
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
		return this->_data + this->_index;
	}

	void ByteBuffer::write(uint8_t byte)
	{
		this->write(&byte, sizeof(byte));
	}

	void ByteBuffer::write(const uint8_t* bytes, size_t num)
	{
		size_t needed, avail;

		if(this->_index + num > this->count()) {
			if(this->_exactfit) {
				avail = this->count() - this->_index;
				needed = num - avail;
				this->grow(needed);
			} else {
				this->grow(num + 10);
			}
		}

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

	void ByteBuffer::grow(const size_t& size)
	{
		uint8_t *old = this->_data;

		this->_data = (uint8_t*)lwiot_mem_zalloc(size + this->count());
		Countable::grow(size);
		memset(this->_data, 0, this->count());

		memcpy(this->_data, old, this->_index);
		lwiot_mem_free(old);
	}
}
