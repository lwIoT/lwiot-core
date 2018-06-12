/*
 * Virtual data stream definition.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>
#include <stdint.h>

#include <lwiot/types.h>
#include <lwiot/log.h>
#include <lwiot/stream.h>

namespace lwiot {
	Stream::Stream() : Stream(1000)
	{
	}

	Stream::Stream(const time_t& timeout)
	{
		this->_millis = 0;
		this->_timeout = timeout;
	}

	ssize_t Stream::read(String& output)
	{
		char x;
		ssize_t bytes;

		bytes = 0;
		while((x = this->read()) != '\n') {
			bytes++;
			output += x;
		}

		return bytes;
	}

	void Stream::write(const String& data)
	{
		this->write((const uint8_t*)data.c_str(), (ssize_t)data.length());
	}
}
