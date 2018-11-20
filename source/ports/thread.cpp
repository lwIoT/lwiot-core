/*
 * Thread wrapper class for the system thread implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <string.h>

#include <lwiot/lwiot.h>
#include <lwiot/thread.h>
#include <lwiot/log.h>

namespace lwiot
{
	void thread_starter(void *arg)
	{
		auto *ctx = (Thread*)arg;
		ctx->run(ctx->argument);
	}

	Thread::Thread(const char *name, void *argument) : internal(), prio(-1), stacksize(0), _name(name)
	{
		this->running = false;
		this->argument = argument;

		print_dbg("Creating thread [%s]!\n", name);
	}

	Thread::Thread(const String& name, int priority, size_t stacksize, void* argument) : internal(), prio(priority), stacksize(stacksize), _name(name)
	{
		this->running = false;
		this->argument = argument;
	}

	Thread::Thread(const lwiot::String &name, void *argument) : Thread(name.c_str(), argument)
	{
	}

	Thread::~Thread()
	{
		if(this->running)
			this->stop();
	}

	void Thread::start()
	{
		lwiot_thread_attributes attrs{};

		this->running = true;

		if(this->prio >= 0 && this->stacksize > 0UL) {
			attrs.name = this->_name.c_str();
			attrs.argument = this;
			attrs.priority = this->prio;
			attrs.stacksize = this->stacksize;
			attrs.handle = thread_starter;

			this->internal = lwiot_thread_create_raw(&attrs);
		} else {
			this->internal = lwiot_thread_create(thread_starter, this->_name.c_str(), this);
		}
	}

	void Thread::stop()
	{
		this->running = false;
		lwiot_thread_destroy(this->internal);
	}
}
