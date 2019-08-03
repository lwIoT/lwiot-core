/*
 * Event wrapper class header.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot_opts.h>

#include <lwiot/kernel/port.h>
#include <lwiot/kernel/lock.h>
#include <lwiot/kernel/uniquelock.h>

#include <lwiot/scopedlock.h>

namespace lwiot {
	class Event {
	public:
		explicit Event(int length = 4);
		explicit Event(const Event& event) = delete;
		Event(Event&& event) noexcept ;
		virtual ~Event();

		Event& operator=(const Event& rhs) = delete;
		Event& operator=(Event&& rhs) noexcept ;

		const lwiot_event_t* getEvent() const;

		void wait();
		bool wait(int tmo);
		bool wait(ScopedLock& guard, int tmo = FOREVER);

		template <typename T>
		bool wait(UniqueLock<T>& guard, int tmo = FOREVER)
		{
			guard.unlock();
			auto rv = this->wait(tmo);
			guard.lock();

			return rv;
		}


		void signalFromIrq();
		void signal();

	private:
		lwiot_event_t* _event;
		int _waiters;
		Lock _lock;
	};
}
