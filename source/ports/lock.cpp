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

namespace lwiot {
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

	void Lock::unlock()
	{
		lwiot_mutex_unlock(&this->mtx);
	}
}
