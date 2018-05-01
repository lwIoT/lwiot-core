/*
 * lwIoT - Mutex header.
 *
 * Author: Michel Megens
 * Date:   30/04/2018
 * Email:  dev@bietje.net
 */

#pragma once

#include <lwiot/port.h>

namespace lwiot {
	class Lock {
	public:
		explicit Lock(bool recursive = false);
		virtual ~Lock();

		void lock();
		bool try_lock(int tmo = 100);
		void unlock();

	private:
		lwiot_mutex_t mtx;
	};
}
