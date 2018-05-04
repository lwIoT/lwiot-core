/*
 * Event wrapper class.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/event.h>
#include <lwiot/log.h>
#include <lwiot/error.h>

namespace lwiot {
	Event::Event(int length)
	{
		lwiot_event_create(&this->_event, length);
	}

	Event::~Event()
	{
		lwiot_event_destroy(&this->_event);
	}

	const lwiot_event_t& Event::getEvent() const
	{
		return this->_event;
	}

	void Event::wait()
	{
		this->wait(FOREVER);
	}

	bool Event::wait(int tmo)
	{
		return lwiot_event_wait(&this->_event, tmo) == -EOK;
	}

	void Event::signal()
	{
		lwiot_event_signal(&this->_event);
	}

	void Event::signalFromIrq()
	{
		lwiot_event_signal_irq(&this->_event);
	}
}
