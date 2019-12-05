/*
 * UniqueLock definition & implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file uniquelock.h

#pragma once

#include <lwiot/sharedpointer.h>

#include <lwiot/kernel/lock.h>
#include <lwiot/stl/referencewrapper.h>

namespace lwiot
{
	/**
	 * @brief Lock implemented according to the RAII-principle.
	 * @tparam T Lock type.
	 * @ingroup kernel
	 * @see Lock
	 */
	template <typename T>
	class UniqueLock {
	public:
		typedef T LockType; //!< Lock type.

		/**
		 * @brief Construct a new RAII lock.
		 * @param lock Lock object.
		 */
		explicit UniqueLock(LockType & lock) : _lock(lock), _locked(false)
		{
			this->lock();
		}

		/**
		 * @brief Construct a new RAII lock.
		 * @param ptr Lock pointer.
		 */
		explicit UniqueLock(SharedPointer<LockType >& ptr) : UniqueLock(ptr.get())
		{
		}

		/**
		 * @brief Construct a new RAII lock.
		 * @param lock Lock pointer.
		 */
		explicit UniqueLock(LockType* lock) : UniqueLock(*lock)
		{
		}

		/**
		 * @brief Move construct a RAII-lock.
		 * @param other Lock to move.
		 */
		UniqueLock(UniqueLock&& other) noexcept : _lock(stl::move(other.lock())), _locked(other._locked)
		{
			other._locked = false;
		}

		/**
		 * @brief UniqueLock destructor.
		 */
		~UniqueLock()
		{
			if(this->_locked) {
				this->unlock();
			}
		}

		/**
		 * @brief Move assignment operator.
		 * @param rhs UniqueLock to move.
		 * @return A reference to \p *this.
		 */
		constexpr UniqueLock& operator=(UniqueLock&& rhs) noexcept
		{
			this->_lock = stl::move(rhs.lock);
			this->_locked = rhs._locked;
			rhs._locked = false;

			return *this;
		}

		LockType & operator=(const LockType & lock) = delete;
		UniqueLock & operator=(const UniqueLock & lock) = delete;

		/**
		 * @brief Lock the underlying lock.
		 */
		void lock() const
		{
			this->_lock->lock();
			this->_locked = true;
		}

		/**
		 * @brief Unlock the underlying lock.
		 */
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
