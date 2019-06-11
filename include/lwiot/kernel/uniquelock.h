/*
 * UniqueLock definition & implementation.
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
	class UniqueLock {
	public:
		typedef T LockType;

		explicit UniqueLock(LockType & lock) : _lock(lock), _locked(false)
		{
			this->lock();
		}

		explicit UniqueLock(SharedPointer<LockType >& ptr) : UniqueLock(ptr.get())
		{
		}

		explicit UniqueLock(LockType* lock) : UniqueLock(*lock)
		{
		}

		~UniqueLock()
		{
			if(this->_locked) {
				this->unlock();
			}
		}

		LockType & operator=(const LockType & lock) = delete;

		void lock() const
		{
			this->_lock->lock();
			this->_locked = true;
		}

		void unlock() const
		{
			this->_locked = false;
			this->_lock->unlock();
		}

	private:
		stl::ReferenceWrapper<LockType> _lock;
		mutable bool _locked;
	};
}
