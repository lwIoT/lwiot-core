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
		this->_timeout = timeout;
	}

	ssize_t Stream::read(String& output)
	{
		int x;
		ssize_t bytes;
		time_t tmo = lwiot_tick_ms() + this->_timeout;

		bytes = 0;

		do {
			x = this->read();

			if(x == static_cast<uint8_t>(EOF) || (this->_timeout > 0 && lwiot_tick_ms() > tmo)) {
				print_dbg("EOF or timeout!\n");
				return bytes;
			}

			bytes++;
			output += static_cast<char>(x);

			if(x == '\n') {
				print_dbg("New line found!\n");
			}
		} while(x != '\n');

		return bytes;
	}

	ssize_t Stream::write(const String& data)
	{
		return this->write((const uint8_t*)data.c_str(), (ssize_t)data.length());
	}

	void Stream::setTimeout(time_t tmo)
	{
		this->_timeout = tmo;
	}

	String Stream::readString()
	{
		String retval;

		this->read(retval);
		return retval;
	}

	String Stream::readStringUntil(char terminator)
	{
		String retval;
		int c;
		time_t tmo = lwiot_tick_ms() + this->_timeout;

		c = this->read();

		while(c >= 0 && c != terminator) {
			if(this->_timeout > 0 && lwiot_tick_ms() > tmo) {
				return String("");
			}

			retval += (char)c;
			c = this->read();
		}


		return retval;
	}
}
