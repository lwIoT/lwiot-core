/*
 * Functional thread implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/function.h>
#include <lwiot/kernel/functionalthread.h>

namespace lwiot
{
	FunctionalThread::FunctionalThread(const String& name) : Thread(name.c_str())
	{
	}

	FunctionalThread::FunctionalThread(const char *name) : Thread(name)
	{
	}

	FunctionalThread::FunctionalThread(const String &name, FunctionalThread::Runner &runner) :
		Thread(name.c_str()), _runner(runner)
	{
	}

	FunctionalThread::FunctionalThread(FunctionalThread &&other) noexcept : Thread(stl::forward<FunctionalThread>(other)),
	                                                                        _runner(stl::move(other._runner))
	{ }

	void FunctionalThread::run()
	{
		if(this->_runner.valid())
			this->_runner();
	}

	void FunctionalThread::start(const lwiot::FunctionalThread::Runner &runner)
	{
		this->_runner = runner;
		Thread::start();
	}

	FunctionalThread &FunctionalThread::operator=(FunctionalThread &&rhs) noexcept
	{
		this->move(rhs);
		this->_runner = stl::move(rhs._runner);

		return *this;
	}

}
