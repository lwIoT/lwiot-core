/*
 * lwIoT - Mutex implementation.
 *
 * Author: Michel Megens
 * Date:   30/04/2018
 * Email:  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/lock.h>
#include <lwiot/log.h>
#include <lwiot/error.h>

namespace lwiot {
	Lock::Lock(bool recursive) : _mtx(new LockValue(recursive))
	{
	}

	void Lock::lock()
	{
		this->_mtx->lock();
	}

	bool Lock::try_lock(int tmo)
	{
		return this->_mtx->try_lock(tmo);
	}

	void Lock::unlock()
	{
		this->_mtx->unlock();
	}
}
