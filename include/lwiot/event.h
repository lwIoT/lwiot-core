/*
 * Event wrapper class header.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot_opts.h>
#include <lwiot/port.h>

namespace lwiot {
	class Event {
	public:
		explicit Event(int length = 4);
		virtual ~Event();

		const lwiot_event_t* getEvent() const;

		void wait();
		bool wait(int tmo);
		void signalFromIrq();
		void signal();

	private:
		lwiot_event_t* _event;
	};
}
