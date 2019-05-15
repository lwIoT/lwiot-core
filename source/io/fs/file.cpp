/*
 * File I/O wrapper class.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */


#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <lwiot.h>

#include <lwiot/stream.h>
#include <lwiot/io/file.h>

namespace lwiot
{
	File::File(const lwiot::String &fname, lwiot::FileMode mode) : _mode(mode)
	{
		char fmode[3] = {0,0,0};

		File::fileModeToString(mode, fmode);
		this->_io = fopen(fname.c_str(), fmode);

		if(this->_io == nullptr)
			return;

		fseek(this->_io, 0L, SEEK_END);
		this->_available = ftell(this->_io);
		fseek(this->_io, 0L, SEEK_SET);
	}

	File::~File()
	{
		if(this->_io == nullptr)
			return;

		fclose(this->_io);
	}

	void File::fileModeToString(lwiot::FileMode mode, char *m)
	{
		const char *tmp;

		switch(mode) {
		case FileMode::Append:
			tmp = "a";
			break;

		case FileMode::Read:
			tmp = "r";
			break;

		case FileMode::Write:
			tmp = "w";
			break;

		case FileMode::ReadWrite:
			tmp = "w+";
			break;

		case FileMode::ReadWriteNoCreate:
			tmp = "r+";
			break;

		case FileMode::ReadWriteAppend:
			tmp = "a+";
			break;

		default:
			tmp = "";
			break;
		}

		strcpy(m, tmp);
	}

	size_t File::available() const
	{
		return this->_available;
	}

	Stream &File::operator<<(char x)
	{
		this->write(&x, sizeof(x));
		return *this;
	}

	Stream &File::operator<<(short x)
	{
		this->write(&x, sizeof(x));
		return *this;
	}

	Stream &File::operator<<(int x)
	{
		this->write(&x, sizeof(x));
		return *this;
	}

	Stream &File::operator<<(const long &x)
	{
		return *this;
	}

	Stream &File::operator<<(const long long &x)
	{
		this->write(&x, sizeof(x));
		return *this;
	}

	Stream &File::operator<<(unsigned char x)
	{
		this->write(&x, sizeof(x));
		return *this;
	}

	Stream &File::operator<<(unsigned short x)
	{
		this->write(&x, sizeof(x));
		return *this;
	}

	Stream &File::operator<<(unsigned int x)
	{
		this->write(&x, sizeof(x));
		return *this;
	}

	Stream &File::operator<<(const unsigned long &x)
	{
		this->write(&x, sizeof(x));
		return *this;
	}

	Stream &File::operator<<(const unsigned long long &x)
	{
		this->write(&x, sizeof(x));
		return *this;
	}

	Stream &File::operator<<(const double &x)
	{
		this->write(&x, sizeof(x));
		return *this;
	}

	Stream &File::operator<<(const float &x)
	{
		this->write(&x, sizeof(x));
		return *this;
	}

	Stream &File::operator<<(const String &str)
	{
		Stream::write(str);
		return *this;
	}

	Stream &File::operator<<(const char *cstr)
	{
		this->writef(cstr);
		return *this;
	}

	uint8_t File::read()
	{
		this->_available--;
		return static_cast<uint8_t>(getc(this->_io));
	}

	ssize_t File::read(void *output, const size_t &length)
	{
		auto bytes = fread(output, length, 1, this->_io) * length;
		this->_available -= bytes;

		return bytes;
	}

	bool File::write(uint8_t byte)
	{
		return fputc(byte, this->_io) != EOF;
	}

	ssize_t File::write(const void *bytes, const size_t &length)
	{
		return fwrite(bytes, length, 1, this->_io) * length;
	}

	ssize_t File::writef(const String &format, ...)
	{
		va_list list;
		ssize_t rv;

		va_start(list, format.c_str());
		rv = vfprintf(this->_io, format.c_str(), list);
		va_end(list);

		return rv;
	}
}
