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
#ifndef NO_OS
	Lock::Lock(bool recursive)
	{
		if(recursive)
			lwiot_mutex_create(&this->mtx, MTX_RECURSIVE);
		else
			lwiot_mutex_create(&this->mtx, 0);
	}

	Lock::~Lock()
	{
		lwiot_mutex_destroy(&this->mtx);
	}

	void Lock::lock()
	{
		lwiot_mutex_lock(&this->mtx, FOREVER);
	}

	bool Lock::try_lock(int tmo)
	{
		return lwiot_mutex_lock(&this->mtx, tmo) == -EOK;
	}

	void Lock::unlock()
	{
		lwiot_mutex_unlock(&this->mtx);
	}
#else /* No OS definitions */
	Lock::Lock(bool recursive) : _lockval(0)
	{
	}

	void Lock::lock()
	{
		while(this->_lockval);
		this->_lockval = 1;
	}

	void Lock::unlock()
	{
		this->_lockval = 0;
	}

	bool Lock::try_lock(int tmo)
	{
		if(this->_lockval)
			return false;

		this->lock();
		return true;
	}
#endif
}
