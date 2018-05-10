/*
 * Virtual data stream definition.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

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

	Stream::~Stream()
	{ }
}
