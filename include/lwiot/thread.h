/*
 *
 */

#pragma once

#include <lwiot.h>

namespace lwiot {
	class Thread {
	public:
		explicit Thread(const char *name, void *argument = nullptr);
		virtual ~Thread();

		void stop();
		void start();
		void kill();

	protected:
		virtual void run(void *arg) = 0;

	private:
		friend void thread_starter(void *arg);

		void *argument;
		bool running;
		lwiot_thread_t internal;
	};

}
