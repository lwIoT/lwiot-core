/*
 * Scoped lock definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <lwiot.h>
#include <stdlib.h>

#include <lwiot/lock.h>
#include <lwiot/log.h>

namespace lwiot
{
	class ScopedLock {
	public:
		explicit ScopedLock(Lock& lock);
		virtual ~ScopedLock();

		ScopedLock& operator=(const ScopedLock& lock) = delete;

		void lock() const;
		void unlock() const;

	private:
		Lock& _lock;
		mutable bool _locked;
	};
}
