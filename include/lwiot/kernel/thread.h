/*
 * lwIoT thread definition.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file thread.h

#pragma once

#include <lwiot.h>

#include <lwiot/kernel/event.h>
#include <lwiot/scopedlock.h>

#include <lwiot/stl/string.h>
#include <lwiot/stl/move.h>

namespace lwiot
{
	/**
	 * @brief Thread wrapper class.
	 * @ingroup kernel
	 */
	class Thread {
	public:
		/**
		 * @brief Construct a new thread object.
		 * @param argument Thread runner argument.
		 */
		explicit Thread(void *argument = nullptr);

		/**
		 * @brief Construct a new thread object.
		 * @param name Thread name.
		 * @param argument Thread runner argument.
		 */
		explicit Thread(const char *name, void *argument = nullptr);

		/**
		 * @brief Construct a new thread object.
		 * @param name Thread name.
		 * @param argument Thread runner argument.
		 */
		explicit Thread(const String& name, void *argument = nullptr);

		/**
		 * @brief Construct a new thread object.
		 * @param name Thread name.
		 * @param priority Thread priority.
		 * @param stacksize Stack size.
		 * @param argument Thread runner argument.
		 */
		explicit Thread(const String& name, int priority, size_t stacksize, void *argument = nullptr);

		/**
		 * @brief Move construct a thread.
		 * @param other Thread to move.
		 */
		Thread(Thread&& other) noexcept ;
		explicit Thread(Thread&) = delete;

		/**
		 * @brief Destroy a thread object.
		 */
		virtual ~Thread();

		void stop(); //!< Stop a thread.
		void start(); //!< Start a thread.
		void join(); //!< Wait for a thread to complete.

		/**
		 * @brief Set the thread name.
		 * @param name Thread name.
		 */
		void setName(const String& name)
		{
			this->_name = name;
		}

		/**
		 * @brief Set the thread name.
		 * @param name Thread name.
		 */
		inline void setName(String&& name)
		{
			this->_name = stl::move(name);
		}

		/**
		 * @brief Assignment operator.
		 * @param rhs Thread to move.
		 * @return A reference to \p *this.
		 */
		Thread& operator=(Thread&& rhs) noexcept ;
		Thread& operator=(Thread& rhs) = delete;

		/**
		 * @brief Yield the current thread.
		 */
		static void yield()
		{
			lwiot_thread_yield();
		}

		/**
		 * @brief Check if a thread is running.
		 * @return True or false based on whether or not a thread is running.
		 */
		inline bool isRunning() const
		{
			return this->_running;
		}

		/**
		 * @brief Sleep the current thread.
		 * @param ms Time to sleep in miliseconds.
		 */
		static void sleep(time_t ms)
		{
			lwiot_sleep(ms);
		}

		/**
		 * @brief Get the threading argument.
		 * @return
		 */
		inline void *argument()
		{
			return this->_argument;
		}

		/**
		 * @brief Swap two threads.
		 * @param a Thread 1.
		 * @param b Thread 2.
		 */
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
		/**
		 * @brief Thread runner.
		 */
		virtual void run() = 0;

		/**
		 * @brief Move a thread.
		 * @param rhs Thread to move.
		 */
		virtual void move(Thread& rhs);

		void *_argument; //!< Thread argument.

	private:
		friend void thread_starter(void *arg);
		void pre_run();

		bool _running;
		bool _allocated;
		lwiot_thread_t *_internal;
		Event _join;
		Lock _lock;

		int _prio;
		size_t stacksize;
		String _name;
	};

}
