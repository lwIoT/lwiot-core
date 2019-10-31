/*
 * Functional thread definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <string.h>
#include <lwiot.h>

#include <lwiot/stl/string.h>
#include <lwiot/function.h>

#include <lwiot/kernel/thread.h>

namespace lwiot
{
	class FunctionalThread : public Thread {
	public:
		typedef Function<void(void)> Runner;

		explicit FunctionalThread(const String& name);
		explicit FunctionalThread(const char* name);
		FunctionalThread(const String& name, Runner& runner);
		FunctionalThread(FunctionalThread&& other) noexcept;
		FunctionalThread(const FunctionalThread& other) = delete;
		~FunctionalThread() override = default;

		FunctionalThread& operator=(FunctionalThread&& rhs) noexcept;
		FunctionalThread& operator=(const FunctionalThread& rhs) = delete;

		template <typename Func>
		FunctionalThread& operator=(Func&& runner)
		{
			this->_runner = stl::forward<Func>(runner);
			return *this;
		}

		void start(const Runner& runner);
		using Thread::start;

		template <typename Func>
		CONSTEXPR void start(Func&& functor)
		{
			this->_runner = stl::forward<Func>(functor);
			Thread::start();
		}

	protected:
		void run() override;

	private:
		Runner _runner;
	};
}
