/*
 * Scoped lock definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/lock.h>
#include <lwiot/scopedlock.h>
#include <lwiot/port.h>

namespace lwiot
{
	ScopedLock::ScopedLock(Lock& lock) : _lock(lock)
	{
		this->_lock.lock();
	}

	ScopedLock::~ScopedLock()
	{
		this->_lock.unlock();
	}
}
