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

#define FUNC_THREAD(__id__, __name__, __lambda__) \
	auto lambda = __lambda__; \
	::lwiot::FunctionalThread<decltype(lambda)> __id__(__name__, lambda)

namespace lwiot
{
	template <class Func>
	class FunctionalThread : public Thread {
	public:
		explicit FunctionalThread(const String& name, Func f) : Thread(name), _func(f)
		{
		}

		explicit FunctionalThread(const char *name, Func f) : Thread(name), _func(f)
		{ }

	protected:
		virtual void run(void *arg) override
		{
			this->_func();
		}

	private:
		Func _func;
	};
}
