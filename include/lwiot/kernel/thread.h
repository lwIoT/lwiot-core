/*
 * lwIoT thread definition.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot.h>
#include <lwiot/stl/string.h>

#include <lwiot/stl/move.h>

namespace lwiot {
	class Thread {
	public:
		explicit Thread(void *argument = nullptr);
		explicit Thread(const char *name, void *argument = nullptr);
		explicit Thread(const String& name, void *argument = nullptr);
		explicit Thread(const String& name, int priority, size_t stacksize, void *argument = nullptr);
		explicit Thread(Thread&& other);
		explicit Thread(Thread&) = delete;
		virtual ~Thread();

		void stop();
		void start();

		void setName(const String& name)
		{
			this->_name = stl::move(name);
		}

		virtual Thread& operator=(Thread&& rhs);
		Thread& operator=(Thread& rhs) = delete;

		static void yield()
		{
			lwiot_thread_yield();
		}

		bool isRunning() const
		{
			return this->running;
		}

	protected:
		virtual void run() = 0;
		void *argument;
		virtual void move(Thread& rhs);

	private:
		friend void thread_starter(void *arg);

		bool running;
		lwiot_thread_t *internal;

		int prio;
		size_t stacksize;
		String _name;
	};

}
