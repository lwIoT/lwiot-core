/*
 * Functional thread definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file functionalthread.h

#pragma once

#include <stdlib.h>
#include <string.h>
#include <lwiot.h>

#include <lwiot/stl/string.h>
#include <lwiot/function.h>

#include <lwiot/kernel/thread.h>

namespace lwiot
{
	/**
	 * @brief Threading as a function.
	 * @ingroup kernel
	 */
	class FunctionalThread : public Thread {
	public:
		typedef Function<void(void)> Runner; //!< Runner type.

		/**
		 * @brief Construct a new functional thread object.
		 * @param name Name.
		 */
		explicit FunctionalThread(const String& name);

		/**
		 * @brief Construct a new functional thread object.
		 * @param name Name.
		 */
		explicit FunctionalThread(const char* name);

		/**
		 * @brief Construct a new functional thread object.
		 * @param name Name.
		 * @param runner Executor.
		 */
		FunctionalThread(const String& name, Runner& runner);

		/**
		 * @brief Move construct a thread.
		 * @param other Functional thread to move.
		 */
		FunctionalThread(FunctionalThread&& other) noexcept;
		FunctionalThread(const FunctionalThread& other) = delete;

		/**
		 * @brief Destruct a functional thread.
		 */
		~FunctionalThread() override = default;

		/**
		 * @brief Move assignment operator.
		 * @param rhs Functional thread to move.
		 * @return A reference to \p *this.
		 */
		FunctionalThread& operator=(FunctionalThread&& rhs) noexcept;
		FunctionalThread& operator=(const FunctionalThread& rhs) = delete;

		/**
		 * @brief Move assignment operator.
		 * @tparam Func Handler type.
		 * @param runner Thread executor.
		 * @return A reference to \p *this.
		 */
		template <typename Func>
		FunctionalThread& operator=(Func&& runner)
		{
			this->_runner = stl::forward<Func>(runner);
			return *this;
		}

		/**
		 * @brief Start the thread.
		 * @param runner Executor.
		 */
		void start(const Runner& runner);
		using Thread::start;

		/**
		 * @brief Start the thread.
		 * @tparam Func Functor type.
		 * @param functor Executor.
		 */
		template <typename Func>
		void start(Func&& functor)
		{
			this->_runner = stl::forward<Func>(functor);
			Thread::start();
		}

	protected:
		void run() override; //!< Thread runner.

	private:
		Runner _runner;
	};
}
