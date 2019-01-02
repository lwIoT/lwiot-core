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
#include <lwiot/functionalthread.h>

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

	FunctionalThread& FunctionalThread::operator=(FunctionalThread::Runner &runner)
	{
		this->_runner = runner;
		return *this;
	}

	void FunctionalThread::run()
	{
		if(this->_runner.valid())
			this->_runner();
	}
}
