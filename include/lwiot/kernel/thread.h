/*
 * lwIoT thread definition.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot.h>

#include <lwiot/kernel/event.h>
#include <lwiot/scopedlock.h>

#include <lwiot/stl/string.h>
#include <lwiot/stl/move.h>

namespace lwiot {
	class Thread {
	public:
		explicit Thread(void *argument = nullptr);
		explicit Thread(const char *name, void *argument = nullptr);
		explicit Thread(const String& name, void *argument = nullptr);
		explicit Thread(const String& name, int priority, size_t stacksize, void *argument = nullptr);
		Thread(Thread&& other) noexcept ;
		explicit Thread(Thread&) = delete;
		virtual ~Thread();

		void stop();
		void start();
		void join();

		void setName(const String& name)
		{
			this->_name = name;
		}

		inline void setName(String&& name)
		{
			this->_name = stl::move(name);
		}

		Thread& operator=(Thread&& rhs) noexcept ;
		Thread& operator=(Thread& rhs) = delete;

		static void yield()
		{
			lwiot_thread_yield();
		}

		inline bool isRunning() const
		{
			return this->_running;
		}

		static void sleep(time_t ms)
		{
			lwiot_sleep(ms);
		}

		inline void *argument()
		{
			return this->_argument;
		}

		friend void swap(Thread& a, Thread& b)
		{
			using stl::swap;
			swap(a._join, b._join);
			swap(a._argument, b._argument);
			swap(a._running, b._running);
			swap(a._internal, b._internal);
			swap(a._prio, b._prio);
			swap(a.stacksize, b.stacksize);
			swap(a._name, b._name);
		}

	protected:
		virtual void run() = 0;
		void *_argument;
		virtual void move(Thread& rhs);

	private:
		friend void thread_starter(void *arg);
		void pre_run();

		bool _running;
		lwiot_thread_t *_internal;
		Event _join;
		Lock _lock;

		int _prio;
		size_t stacksize;
		String _name;
	};

}
