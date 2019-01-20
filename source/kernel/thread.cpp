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
		ctx->pre_run();
	}

	Thread::Thread(void *argument) : Thread("", argument)
	{
	}

	Thread::Thread(const char *name, void *argument) :_prio(-1), stacksize(0), _name(name)
	{
		print_dbg("Creating thread [%s]!\n", name);

		this->_running = false;
		this->_argument = argument;
	}

	Thread::Thread(const String& name, int priority, size_t stacksize, void* argument) : _prio(priority), stacksize(stacksize), _name(name)
	{
		this->_running = false;
		this->_argument = argument;
	}

	Thread::Thread(const lwiot::String &name, void *argument) : Thread(name.c_str(), argument)
	{
	}

	Thread::Thread(lwiot::Thread &&other) :
		_running(other._running), _internal(other._internal), _prio(other._prio),
		stacksize(other.stacksize), _name(other._name)
	{
	}

	Thread::~Thread()
	{
		if(this->_running)
			this->stop();
	}

	Thread& Thread::operator=(lwiot::Thread &&rhs)
	{
		this->move(rhs);
		return *this;
	}

	void Thread::move(lwiot::Thread &rhs)
	{
		if(this->_running) {
			stl::swap(this->_running, rhs._running);
			stl::swap(this->_internal, rhs._internal);
			stl::swap(this->_prio, rhs._prio);
			stl::swap(this->stacksize, rhs.stacksize);
			stl::swap(this->_name, rhs._name);
		} else {
			this->_running = rhs._running;
			this->_internal = rhs._internal;
			this->_prio = rhs._prio;
			this->stacksize = rhs.stacksize;
			this->_name = stl::move(rhs._name);
		}
	}

	void Thread::start()
	{
		lwiot_thread_attributes attrs{};

		this->_running = true;
		assert(this->_name.length() != 0);

		if(this->_prio >= 0 && this->stacksize > 0UL) {
			attrs.name = this->_name.c_str();
			attrs.argument = this;
			attrs.priority = this->_prio;
			attrs.stacksize = this->stacksize;
			attrs.handle = thread_starter;

			this->_internal = lwiot_thread_create_raw(&attrs);
		} else {
			this->_internal = lwiot_thread_create(thread_starter, this->_name.c_str(), this);
		}
	}

	void Thread::join()
	{
		ScopedLock g(this->_lock);

		if(this->_running)
			this->_join.wait(g, FOREVER);
	}

	void Thread::stop()
	{
		ScopedLock g(this->_lock);
		this->_running = false;
		this->_join.signal();

		g.unlock();
		lwiot_thread_destroy(this->_internal);
	}

	void Thread::pre_run()
	{
		this->run();

		ScopedLock g(this->_lock);
		this->_running = false;
		this->_join.signal();
	}
}
