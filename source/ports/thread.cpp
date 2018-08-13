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

	Thread::Thread(const char *name, void *argument) : internal(), prio(-1), stacksize(0)
	{
		this->running = false;
		this->argument = argument;

		memset((void*)this->internal.name, 0, sizeof(this->internal.name));
		memcpy((void*)this->internal.name, name, strlen(name));

		print_dbg("Creating thread [%s]!\n", name);
	}

	Thread::Thread(const String& name, int priority, size_t stacksize, void* argument) :
		internal(), prio(priority), stacksize(stacksize)
	{
		this->running = false;
		this->argument = argument;

		memset((void*)this->internal.name, 0, sizeof(this->internal.name));
		memcpy((void*)this->internal.name, name.c_str(), name.length());
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
			attrs.name = this->internal.name;
			attrs.argument = this;
			attrs.priority = this->prio;
			attrs.stacksize = this->stacksize;
			attrs.handle = thread_starter;

			lwiot_thread_create_raw(&this->internal, &attrs);
		} else {
			lwiot_thread_create(&this->internal, thread_starter, this);
		}
	}

	void Thread::stop()
	{
		this->running = false;
		lwiot_thread_destroy(&this->internal);
	}
}
