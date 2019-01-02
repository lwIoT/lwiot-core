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

#include <lwiot/thread.h>
#include <lwiot/string.h>
#include <lwiot/function.h>

namespace lwiot
{
	class FunctionalThread : public Thread {
	public:
		typedef Function<void(*)(void)> Runner;

		explicit FunctionalThread(const String& name);
		explicit FunctionalThread(const char* name);
		FunctionalThread(const String& name, Runner& runner);
		virtual ~FunctionalThread() = default;

		FunctionalThread& operator=(Runner& runner);

	protected:
		virtual void run() override;

	private:
		Runner _runner;
	};
}
