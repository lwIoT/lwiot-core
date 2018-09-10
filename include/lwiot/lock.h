/*
 * lwIoT - Mutex header.
 *
 * Author: Michel Megens
 * Date:   30/04/2018
 * Email:  dev@bietje.net
 */

#pragma once

#include <lwiot/port.h>
#include <lwiot/types.h>

namespace lwiot {
	class Lock {
	public:
		explicit Lock(bool recursive = false);
		virtual ~Lock();

		explicit Lock(Lock& lock) = delete;
		Lock& operator =(Lock& lock) = delete;

		void lock();
		bool try_lock(int tmo = 100);
		void unlock();

	private:
#ifndef NO_OS
		lwiot_mutex_t mtx;
#else
		bool _lockval;
#endif
	};
}
