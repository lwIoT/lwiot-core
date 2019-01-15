/*
 * Event wrapper class header.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot_opts.h>
#include <lwiot/kernel/port.h>
#include <lwiot/scopedlock.h>

namespace lwiot {
	class Event {
	public:
		explicit Event(int length = 4);
		explicit Event(const Event& event) = delete;
		explicit Event(Event&& event);
		virtual ~Event();

		Event& operator=(const Event& rhs) = delete;
		Event& operator=(Event&& rhs);

		const lwiot_event_t* getEvent() const;

		void wait();
		bool wait(int tmo);
		bool wait(ScopedLock& guard, int tmo = FOREVER);

		void signalFromIrq();
		void signal();

	private:
		lwiot_event_t* _event;
	};
}
