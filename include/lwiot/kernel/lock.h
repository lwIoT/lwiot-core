/*
 * lwIoT - Mutex header.
 *
 * Author: Michel Megens
 * Date:   30/04/2018
 * Email:  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <string.h>
#include <lwiot.h>

#include <lwiot/error.h>
#include <lwiot/stl/string.h>
#include <lwiot/kernel/atomic.h>
#include <lwiot/uniquepointer.h>

namespace lwiot {
	class Lock {
	public:
		explicit Lock(bool recursive = false);
		virtual ~Lock() = default;

		Lock(Lock &&rhs) noexcept : _mtx(stl::move(rhs._mtx))
		{
		}

		Lock& operator=(Lock&& rhs) noexcept
		{
			this->_mtx = stl::move(rhs._mtx);
			return *this;
		}

		explicit Lock(Lock& lock) = delete;
		Lock& operator =(Lock& lock) = delete;

		void lock();
		bool try_lock(int tmo = 100);
		void unlock();

		friend void swap(Lock& a, Lock& b)
		{
			using stl::swap;
			swap(a._mtx, b._mtx);
		}

	private:
		struct LockValue {
		public:
#ifdef CONFIG_STANDALONE
			explicit LockValue(bool value) : _lockval(value)
			{
			}

			void lock()
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

			bool try_lock(int tmo = 100)
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

			void unlock()
			{
				enter_critical();
				this->_lockval = false;
				exit_critical();
			}

			bool _lockval;
#else
			explicit LockValue(bool recursive) : _lockdepth(0)
			{
				if(recursive)
					this->_lock = lwiot_mutex_create(MTX_RECURSIVE);
				else
					this->_lock = lwiot_mutex_create(0);
			}

			virtual ~LockValue()
			{
				lwiot_mutex_destroy(this->_lock);
			}

			void lock()
			{
				lwiot_mutex_lock(this->_lock, FOREVER);
				++this->_lockdepth;
			}

			bool try_lock(int tmo)
			{
				auto result = lwiot_mutex_lock(this->_lock, tmo) == -EOK;

				if(result)
					this->_lockdepth += 1;

				return result;
			}

			void unlock()
			{
				assert(this->_lockdepth >= 0);

				if(this->_lockdepth == 0)
					return;
					

				--this->_lockdepth;
				lwiot_mutex_unlock(this->_lock);
			}

			lwiot_mutex_t* _lock;
			int _lockdepth;
#endif
		};

		UniquePointer<LockValue> _mtx;
	};
}
