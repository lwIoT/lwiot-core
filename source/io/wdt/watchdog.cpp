/*
 * Base watchdog implementation.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdint.h>

#include <lwiot/lwiot.h>
#include <lwiot/io/watchdog.h>


namespace lwiot
{
	Watchdog::Watchdog() : _enabled(false)
	{
	}

	bool Watchdog::enable(uint32_t tmo)
	{
		this->_enabled = true;
		return true;
	}

	bool Watchdog::disable()
	{
		this->_enabled = false;
		return true;
	}

	bool Watchdog::enabled() const
	{
		return this->_enabled;
	}
}
