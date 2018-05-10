/*
 * Buffered byte stream implementation
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>

#include <lwiot/log.h>
#include <lwiot/stream.h>
#include <lwiot/string.h>
#include <lwiot/bufferedstream.h>

namespace lwiot {
	BufferedStream::BufferedStream(int size) : Stream(), _size(size)
	{
		this->_data = new uint8_t[size];
		this->rd_idx = 0;
		this->wr_idx = 0;
	}

	BufferedStream::BufferedStream() : BufferedStream(32)
	{ }

	BufferedStream::~BufferedStream()
	{
		delete [] this->_data;
	}

	uint8_t BufferedStream::read()
	{
		return this->_data[this->rd_idx++];
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
		return this->_size;
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

	uint8_t& BufferedStream::operator[](size_t indx)
	{
		if(indx >= this->_size) {
			print_dbg("Array out of bounds error!\n");
		}

		return this->_data[indx];
	}

	void BufferedStream::append(void* data, size_t length)
	{
		if((length + this->wr_idx) > this->_size) {
			const auto newsize = this->wr_idx + length;
			if(newsize < this->_size * 2U)
				this->grow();
			else
				this->grow(newsize);
		}

		memcpy(this->_data + this->wr_idx, data, length);
		this->wr_idx += length;
	}

	void BufferedStream::grow()
	{
		this->grow(this->_size * 2U);
	}

	void BufferedStream::grow(int num)
	{
		uint8_t *buf;
		auto newsize = num;

		newsize += this->_size;
		buf = new uint8_t[newsize];
		memcpy(buf, this->_data, this->_size);
		delete [] this->_data;
		this->_data = buf;
		this->_size = newsize;
	}

	String BufferedStream::toString()
	{
		this->_data[this->wr_idx] = '\0';
		String str((const char*)this->_data);

		return str;
	}
}
