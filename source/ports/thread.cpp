/*
 * Thread wrapper class for the system thread implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <lwiot/lwiot.h>
#include <lwiot/kernel/thread.h>
#include <lwiot/log.h>

#include <lwiot/stl/move.h>

namespace lwiot
{
	void thread_starter(void *arg)
	{
		auto *ctx = (Thread*)arg;
		ctx->run();
	}

	Thread::Thread(void *argument) : Thread("", argument)
	{
	}

	Thread::Thread(const char *name, void *argument) :prio(-1), stacksize(0), _name(name)
	{
		print_dbg("Creating thread [%s]!\n", name);

		this->running = false;
		this->argument = argument;
	}

	Thread::Thread(const String& name, int priority, size_t stacksize, void* argument) : prio(priority), stacksize(stacksize), _name(name)
	{
		this->running = false;
		this->argument = argument;
	}

	Thread::Thread(const lwiot::String &name, void *argument) : Thread(name.c_str(), argument)
	{
	}

	Thread::Thread(lwiot::Thread &&other) :
		running(other.running), internal(other.internal), prio(other.prio),
		stacksize(other.stacksize), _name(other._name)
	{
	}

	Thread::~Thread()
	{
		if(this->running)
			this->stop();
	}

	Thread& Thread::operator=(lwiot::Thread &&rhs)
	{
		this->move(rhs);
		return *this;
	}

	void Thread::move(lwiot::Thread &rhs)
	{
		if(this->running) {
			stl::swap(this->running, rhs.running);
			stl::swap(this->internal, rhs.internal);
			stl::swap(this->prio, rhs.prio);
			stl::swap(this->stacksize, rhs.stacksize);
			stl::swap(this->_name, rhs._name);
		} else {
			this->running = rhs.running;
			this->internal = rhs.internal;
			this->prio = rhs.prio;
			this->stacksize = rhs.stacksize;
			this->_name = stl::move(rhs._name);
		}
	}

	void Thread::start()
	{
		lwiot_thread_attributes attrs{};

		this->running = true;
		assert(this->_name.length() != 0);

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
