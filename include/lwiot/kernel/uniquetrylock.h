/*
 * UniqueTryLock definition & implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file uniquetrylock.h

#pragma once

#include <lwiot/sharedpointer.h>

#include <lwiot/kernel/lock.h>
#include <lwiot/stl/referencewrapper.h>

namespace lwiot
{
	/**
	 * @brief Attempt to aquire a lock RAII-style.
	 * @tparam T Lock type.
	 * @ingroup kernel
	 */
	template <typename T>
	class UniqueTryLock {
	public:
		typedef T LockType; //!< Lock type.

		/**
		 * @brief Construct a new RAII lock.
		 * @param lock Lock object.
		 * @param tmo Timeout value.
		 */
		explicit UniqueTryLock(LockType & lock, uint32_t tmo = 1000) : _lock(lock), _locked(false)
		{
			if(this->_lock->try_lock(tmo)) {
				this->_locked = true;
			}
		}

		/**
		 * @brief Construct a new RAII lock.
		 * @param ptr Lock pointer.
		 */
		explicit UniqueTryLock(SharedPointer<LockType >& ptr) : UniqueTryLock(ptr.get())
		{
		}

		/**
		 * @brief Construct a new RAII lock.
		 * @param lock Lock pointer.
		 */
		explicit UniqueTryLock(LockType* lock) : UniqueTryLock(*lock)
		{
		}

		/**
		 * @brief Destroy and unlock a UniqueTryLock.
		 */
		~UniqueTryLock()
		{
			if(this->_locked) {
				this->_lock->unlock();
				this->_locked = false;
			}
		}

		/**
		 * @brief Check if the lock has been locked.
		 * @return Lock indicator.
		 */
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
