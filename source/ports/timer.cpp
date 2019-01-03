/*
 * lwIoT timer implementation
 * 
 * Author: Michel Megens
 * Date:   01/05/2018
 * Email:  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/timer.h>
#include <lwiot/string.h>
#include <lwiot/types.h>
#include <lwiot/stl/move.h>

namespace lwiot {
	void run_timer(lwiot_timer_t *t, void *arg)
	{
		Timer *timer = (Timer*)arg;

		if(timer == nullptr)
			return;

		timer->tick();
	}

	Timer::Timer(const String& name, unsigned long ms, uint32_t flags, void *arg)
		: Timer(name.c_str(), ms, flags, arg)
	{
	}

	Timer::Timer(const char *name, unsigned long ms, uint32_t flags, void *arg)
		: argument(arg), running(false)
	{
		this->timer = lwiot_timer_create(name, (int)ms, flags, this, run_timer);
	}

	Timer::Timer(lwiot::Timer&& rhs) : argument(rhs.argument), timer(rhs.timer), running(rhs.running)
	{
	}

	Timer::~Timer()
	{
		lwiot_timer_stop(this->timer);
		lwiot_timer_destroy(this->timer);
	}

	void Timer::reset()
	{
		lwiot_timer_reset(this->timer);
	}

	Timer& Timer::operator=( lwiot::Timer &&rhs)
	{
		if(this->running)
			this->stop();

		this->running = rhs.running;
		this->argument = rhs.argument;
		this->timer = rhs.timer;
		return *this;
	}

	time_t Timer::expiry()
	{
		return lwiot_timer_get_expiry(this->timer);
	}

	void Timer::start()
	{
		this->running = true;
		lwiot_timer_start(this->timer);
	}

	void Timer::stop()
	{
		lwiot_timer_stop(this->timer);
		this->running = false;
	}

	bool Timer::isExpired()
	{
		auto expiry = lwiot_timer_get_expiry(this->timer);
		return expiry <= lwiot_tick();
	}
}
