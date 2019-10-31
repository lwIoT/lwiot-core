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

	ByteBuffer::ByteBuffer(const size_t& size, bool exactfit) : Countable(size), _index(0), _exactfit(exactfit), _default(0)
	{
		if(size == 0UL) {
			this->_data = nullptr;
			return;
		}

		this->_data = (uint8_t*)lwiot_mem_alloc(size);
		memset(this->_data, 0, size);
	}

	ByteBuffer::ByteBuffer(const ByteBuffer& other) : ByteBuffer(other.count())
	{
		memcpy(this->_data, other._data, other.count());
		this->_count = other.count();
		this->_index = other._index;
	}

	ByteBuffer::ByteBuffer(ByteBuffer&& other) noexcept :
		Countable(other.count()), _index(0), _data(nullptr), _exactfit(other._exactfit), _default(0)
	{
		this->move(other);
	}

	ByteBuffer::~ByteBuffer()
	{
		if(this->_data)
			lwiot_mem_free(this->_data);
	}

	ByteBuffer& ByteBuffer::operator=(ByteBuffer&& other) noexcept
	{
		this->move(other);
		return *this;
	}

	void ByteBuffer::reset()
	{
		memset(this->_data, 0, this->_count);
		this->_index = 0;
	}

	void ByteBuffer::move(ByteBuffer& other)
	{
		if(likely(this->_data))
			lwiot_mem_free(this->_data);

		this->_data = other.begin();
		this->_index = other._index;
		this->_count = other.count();

		other._count = 0;
		other._index = 0;
		other._data = nullptr;
	}

	void ByteBuffer::copy(const lwiot::ByteBuffer &other)
	{
		this->reset(other.count());
		this->write(other);
	}

	void ByteBuffer::reset(const size_t& newsize)
	{
		if(newsize == this->count()) {
			memset(this->_data, 0, this->count());
		} else {
			lwiot_mem_free(this->_data);
			this->_data = static_cast<uint8_t *>(lwiot_mem_zalloc(newsize));
			this->_count = newsize;
		}

		this->_index = 0UL;
		this->_exactfit = false;
	}

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
		if(unlikely(this->_data == nullptr))
			return this->_default;

		return this->_data[idx];
	}

	const uint8_t& ByteBuffer::operator[](const size_t& idx) const
	{
		if(unlikely(this->_data == nullptr))
			return this->_default;

		return this->_data[idx];
	}

	uint8_t* ByteBuffer::begin() const
	{
		return this->_data;
	}

	uint8_t* ByteBuffer::end() const
	{
		return this->_data + this->_index;
	}

	void ByteBuffer::write(uint8_t byte)
	{
		this->write(&byte, sizeof(byte));
	}

	void ByteBuffer::write(const lwiot::RawBuffer &raw)
	{
		this->write((const uint8_t*)raw.buffer(), raw.size());
	}

	void ByteBuffer::write(const lwiot::ByteBuffer &buffer)
	{
		this->write(buffer.data(), buffer.index());
	}

	void ByteBuffer::write(const void* data, size_t num)
	{
		size_t needed, avail;
		const uint8_t *bytes = (const uint8_t*) data;

		if(unlikely(bytes == nullptr)) {
			auto size = num >= BYTEBUFFER_DEFAULT_SIZE ? num : BYTEBUFFER_DEFAULT_SIZE;
			this->_data = static_cast<uint8_t *>(lwiot_mem_zalloc(size));
			this->_count = BYTEBUFFER_DEFAULT_SIZE;
			this->_index = 0;
		}

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

	bool ByteBuffer::operator !=(const ByteBuffer& rhs) const
	{
		return !(*this == rhs);
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
