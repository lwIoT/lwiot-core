/*
 *
 */

#pragma once

#include <lwiot.h>
#include <lwiot/string.h>

namespace lwiot {
	class Thread {
	public:
		explicit Thread(const char *name, void *argument = nullptr);
		explicit Thread(const String& name, void *argument = nullptr);
		virtual ~Thread();

		void stop();
		void start();

		bool isRunning()
		{
			return this->running;
		}

	protected:
		virtual void run(void *arg) = 0;

	private:
		friend void thread_starter(void *arg);

		void *argument;
		bool running;
		lwiot_thread_t internal;
	};

}
