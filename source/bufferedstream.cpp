/*
 * Buffered byte stream implementation
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <string.h>

#include <lwiot/log.h>
#include <lwiot/stream.h>
#include <lwiot/string.h>
#include <lwiot/bufferedstream.h>

namespace lwiot {
	BufferedStream::BufferedStream(int size) : Stream(), Countable(size)
	{
		this->_data = (uint8_t*)lwiot_mem_zalloc(size);
		this->rd_idx = 0;
		this->wr_idx = 0;
	}

	BufferedStream::BufferedStream() : BufferedStream(32)
	{ }

	BufferedStream::~BufferedStream()
	{
		lwiot_mem_free(this->_data);
	}

	uint8_t BufferedStream::read()
	{
		return this->_data[this->rd_idx++];
	}

	ssize_t BufferedStream::read(uint8_t *buffer, const size_t& length)
	{
		auto max = this->length();
		size_t to_read;

		if(this->rd_idx + length > max) {
			to_read = max;
		} else {
			to_read = length;
		}

		memcpy((void*)buffer, this->_data, to_read);
		this->rd_idx += to_read;

		return to_read;
	}

	ssize_t BufferedStream::read(String& data)
	{
		String us(this->toString());
		data.concat(us);

		return us.length();
	}

	void BufferedStream::write(uint8_t byte)
	{
		*this << byte;
	}

	void BufferedStream::write(const uint8_t *data, const size_t& length)
	{
		this->append((const void*)data, length);
	}

	size_t BufferedStream::available()
	{
		auto written = this->wr_idx;

		written -= this->rd_idx;
		return written;
	}

	const void *BufferedStream::data() const
	{
		return this->_data;
	}

	const size_t& BufferedStream::size() const
	{
		return this->count();
	}

	Stream& BufferedStream::operator<<(char x)
	{
		this->append((void*)&x, sizeof(x));
		return *this;
	}

	Stream& BufferedStream::operator<<(short x)
	{
		this->append((void*)&x, sizeof(x));
		return *this;
	}

	Stream& BufferedStream::operator<<(int x)
	{
		this->append((void*)&x, sizeof(x));
		return *this;
	}

	Stream& BufferedStream::operator<<(const long& x)
	{
		this->append((void*)&x, sizeof(x));
		return *this;
	}

	Stream& BufferedStream::operator<<(const long long& x)
	{
		this->append((void*)&x, sizeof(x));
		return *this;
	}

	Stream& BufferedStream::operator<<(unsigned char x)
	{
		this->append((void*)&x, sizeof(x));
		return *this;
	}

	Stream& BufferedStream::operator<<(unsigned int x)
	{
		this->append((void*)&x, sizeof(x));
		return *this;
	}

	Stream& BufferedStream::operator<<(unsigned short x)
	{
		this->append((void*)&x, sizeof(x));
		return *this;
	}

	Stream& BufferedStream::operator<<(const unsigned long long& x)
	{
		this->append((void*)&x, sizeof(x));
		return *this;
	}

	Stream& BufferedStream::operator<<(const unsigned long& x)
	{
		this->append((void*)&x, sizeof(x));
		return *this;
	}

	Stream& BufferedStream::operator<<(const double& flt)
	{
		this->append((void*)&flt, sizeof(flt));
		return *this;
	}

	Stream& BufferedStream::operator<<(const float& flt)
	{
		this->append((void*)&flt, sizeof(flt));
		return *this;
	}

	Stream& BufferedStream::operator<<(const char *cstr)
	{
		this->append((void*)cstr, strlen(cstr));
		return *this;
	}

	Stream& BufferedStream::operator<<(const String& str)
	{
		this->append((void*)str.c_str(), str.length());
		return *this;
	}

	uint8_t& BufferedStream::operator[](const size_t& indx)
	{
		if(indx >= this->count()) {
			print_dbg("Array out of bounds error!\n");
		}

		return this->_data[indx];
	}

	const uint8_t& BufferedStream::operator[](const size_t& idx) const
	{
		if(idx >= this->count()) {
			print_dbg("Array out of bounds error!\n");
		}

		return this->_data[idx];
	}

	size_t BufferedStream::length() const
	{
		return this->wr_idx;
	}

	void BufferedStream::append(const void* data, size_t length)
	{
		if((length + this->wr_idx) > this->count()) {
			const auto newsize = this->wr_idx + length;
			if(newsize < this->count() * 2U)
				this->grow();
			else
				this->grow(newsize);
		}

		memcpy(this->_data + this->wr_idx, data, length);
		this->wr_idx += length;
	}

	void BufferedStream::grow()
	{
		this->grow(this->count() * 2U);
	}

	void BufferedStream::grow(int num)
	{
		uint8_t *buf;
		auto newsize = num;

		Countable::grow(static_cast<size_t>(num));
		newsize += this->count();
		buf = (uint8_t*) lwiot_mem_zalloc(newsize);
		memcpy(buf, this->_data, this->count());
		lwiot_mem_free(this->_data);
		this->_data = buf;
	}

	String BufferedStream::toString()
	{
		this->_data[this->wr_idx] = '\0';
		String str((const char*)this->_data);

		return str;
	}
}
