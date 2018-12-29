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
		this->_event = lwiot_event_create(length);
	}

	Event::Event(lwiot::Event &&event)
	{
		this->_event = event._event;
		event._event = nullptr;
	}

	Event::~Event()
	{
		lwiot_event_destroy(this->_event);
	}

	Event& Event::operator=(lwiot::Event &&rhs)
	{
		this->_event = rhs._event;
		rhs._event = nullptr;

		return *this;
	}

	const lwiot_event_t* Event::getEvent() const
	{
		return this->_event;
	}

	void Event::wait()
	{
		this->wait(FOREVER);
	}

	bool Event::wait(int tmo)
	{
		if(!this->_event)
			return false;

		return lwiot_event_wait(this->_event, tmo) == -EOK;
	}

	bool Event::wait(lwiot::ScopedLock &guard, int tmo)
	{
		guard.unlock();
		auto rv = this->wait(tmo);
		guard.lock();

		return rv;
	}

	void Event::signal()
	{
		if(!this->_event)
			return;

		lwiot_event_signal(this->_event);
	}

	void Event::signalFromIrq()
	{
		if(!this->_event)
			return;

		lwiot_event_signal_irq(this->_event);
	}
}
