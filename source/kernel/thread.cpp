/*
 * Thread wrapper class for the system thread implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

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

	Thread::Thread(const char *name, void *argument) : _allocated(false),_internal(nullptr),
		_prio(-1), stacksize(0), _name(name)
	{
		this->_running = false;
		this->_argument = argument;
	}

	Thread::Thread(const String& name, int priority, size_t stacksize, void* argument) :
		_argument(nullptr), _running(false), _allocated(false),
		_internal(nullptr), _prio(priority), stacksize(stacksize), _name(name)
	{
		this->_running = false;
		this->_argument = argument;
	}

	Thread::Thread(const lwiot::String &name, void *argument) : Thread(name.c_str(), argument)
	{
	}

	Thread::Thread(lwiot::Thread &&other) noexcept :
		_argument(other._argument), _running(other._running), _allocated(other._allocated), _internal(other._internal),
		_join(stl::move(other._join)), _lock(stl::move(other._lock)), _prio(other._prio), stacksize(other.stacksize),
		_name(stl::move(other._name))
	{
	}

	Thread::~Thread()
	{
		ScopedLock g(this->_lock);

		if(this->_running) {
			g.unlock();
			this->stop();
		}
	}

	Thread& Thread::operator=(lwiot::Thread &&rhs) noexcept
	{
		this->move(rhs);
		return *this;
	}

	void Thread::move(lwiot::Thread &rhs)
	{
		ScopedLock l1(this->_lock);
		ScopedLock l2(rhs._lock);

		using stl::swap;
		swap(*this, rhs);
	}

	void Thread::start()
	{
		lwiot_thread_attributes attrs{};

		print_dbg("Creating thread [%s]!\n", this->_name.c_str());
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

		this->_allocated = true;
	}

	void Thread::join()
	{
		ScopedLock g(this->_lock);

		if(this->_running) {
			this->_join.wait(g, FOREVER);
			lwiot_thread_destroy(this->_internal);
		}
	}

	void Thread::stop()
	{
		ScopedLock g(this->_lock);

		if(!this->_allocated)
			return;

		this->_running = false;
		this->_allocated = false;
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
