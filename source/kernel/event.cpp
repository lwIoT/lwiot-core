/*
 * Event wrapper class.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/kernel/event.h>
#include <lwiot/log.h>
#include <lwiot/error.h>

namespace lwiot {
	Event::Event(int length) : _event(nullptr), _waiters(0)
	{
		this->_event = lwiot_event_create(length);
	}

	Event::Event(lwiot::Event &&event) : _event(nullptr), _waiters(0)
	{
		this->_event = event._event;
		event._event = nullptr;
	}

	Event::~Event()
	{
		lwiot_event_destroy(this->_event);
		this->_event = nullptr;
	}

	Event& Event::operator=(lwiot::Event &&rhs)
	{
		this->_event = rhs._event;
		rhs._event = nullptr;
		this->_waiters = rhs._waiters;
		this->_lock = stl::move(rhs._lock);

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

		this->_lock.lock();
		++this->_waiters;
		this->_lock.unlock();

		auto result = lwiot_event_wait(this->_event, tmo) == -EOK;

		this->_lock.lock();
		--this->_waiters;
		this->_lock.unlock();

		return result;
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

		ScopedLock g(this->_lock);
		if(this->_waiters == 0)
			return;

		g.unlock();
		lwiot_event_signal(this->_event);
	}

	void Event::signalFromIrq()
	{
		if(!this->_event)
			return;

		if(this->_waiters == 0)
			return;

		lwiot_event_signal_irq(this->_event);
	}
}
