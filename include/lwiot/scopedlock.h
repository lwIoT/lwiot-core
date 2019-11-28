/*
 * Scoped lock definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot.h>
#include <stdlib.h>

#include <lwiot/log.h>
#include <lwiot/sharedpointer.h>

#include <lwiot/kernel/lock.h>
#include <lwiot/stl/referencewrapper.h>

namespace lwiot
{
	/**
	 * @brief RAII-style locking mechanism.
	 * @ingroup util
	 */
	class ScopedLock {
	public:
		/**
		 * @brief Construct a new lock.
		 * @note This will acquire \p lock.
		 * @param lock Underlying lock.
		 */
		explicit ScopedLock(Lock& lock);
		/**
		 * @brief Construct a new lock.
		 * @note This will acquire \p lock.
		 * @param lock Underlying lock.
		 */
		explicit ScopedLock(Lock* lock);
		/**
		 * @brief Construct a new lock.
		 * @note This will acquire \p lock.
		 * @param sp Underlying lock.
		 */
		explicit ScopedLock(SharedPointer<Lock>& sp);

		/**
		 * @brief Destroy a lock.
		 * @note This will unlock the underlying lock.
		 */
		virtual ~ScopedLock();

		ScopedLock& operator=(const ScopedLock& lock) = delete;

		/**
		 * @brief Lock the underlying lock.
		 */
		void lock() const;

		/**
		 * @brief Unlock the underlying lock.
		 */
		void unlock() const;

	private:
		stl::ReferenceWrapper<Lock> _lock;
		mutable bool _locked;
	};
}
