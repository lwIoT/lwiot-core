/*
 * lwIoT timer implementation
 * 
 * Author: Michel Megens
 * Date:   01/05/2018
 * Email:  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <string.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/error.h>
#include <lwiot/string.h>

namespace lwiot {
	class Timer {
	public:
		explicit Timer(const lwiot::String& name, unsigned long ms, uint32_t flags, void *arg);
		explicit Timer(const char *name, unsigned long ms, uint32_t flags, void *arg);
		explicit Timer(const Timer&) = delete;
		explicit Timer(Timer&& other);
		virtual ~Timer();

		Timer& operator=(const Timer&) = delete;
		Timer& operator=(Timer&& rhs);

		void start();
		void stop();
		bool isExpired();
		void reset();

		time_t expiry();

	protected:
		virtual void tick() = 0;
		void *argument;

	private:
		friend void run_timer(lwiot_timer_t *t, void *argument);
		lwiot_timer_t* timer;
		bool running;
	};
}
