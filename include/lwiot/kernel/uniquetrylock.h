/*
 * UniqueTryLock definition & implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/sharedpointer.h>

#include <lwiot/kernel/lock.h>
#include <lwiot/stl/referencewrapper.h>

namespace lwiot
{
	template <typename T>
	class UniqueTryLock {
	public:
		typedef T LockType;

		explicit UniqueTryLock(LockType & lock, uint32_t tmo = 1000) : _lock(lock), _locked(false)
		{
			if(this->_lock->try_lock(tmo)) {
				this->_locked = true;
			}
		}

		explicit UniqueTryLock(SharedPointer<LockType >& ptr) : UniqueTryLock(ptr.get())
		{
		}

		explicit UniqueTryLock(LockType* lock) : UniqueTryLock(*lock)
		{
		}

		~UniqueTryLock()
		{
			if(this->_locked) {
				this->_lock->unlock();
				this->_locked = false;
			}
		}

		bool locked() const
		{
			return this->_locked;
		}

		LockType & operator=(const LockType & lock) = delete;

	private:
		stl::ReferenceWrapper<LockType> _lock;
		mutable volatile bool _locked;
	};
}
