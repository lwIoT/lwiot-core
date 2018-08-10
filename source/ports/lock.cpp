/*
 * lwIoT - Mutex implementation.
 *
 * Author: Michel Megens
 * Date:   30/04/2018
 * Email:  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>
#include <cpu.h>

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
	Lock::Lock(bool recursive) : _lockval(false)
	{
	}

	void Lock::lock()
	{
		bool value = true;

		while(value) {
			enter_critical();
			value = this->_lockval;
			exit_critical();

			lwiot_sleep(1);
		}

		enter_critical();
		this->_lockval = true;
		exit_critical();
	}

	void Lock::unlock()
	{
		enter_critical();
		this->_lockval = false;
		exit_critical();
	}

	bool Lock::try_lock(int tmo)
	{
		bool cache;

		enter_critical();
		cache = this->_lockval;
		exit_critical();

		if(cache)
			return false;

		this->lock();
		return true;
	}
#endif
}
