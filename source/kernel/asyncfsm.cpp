/*
 * Asynchronous FSM implementation.
 *
 * @author Michel Megens
 * @email  michel@michelmegens.net
 */

#include <lwiot.h>

#include <lwiot/kernel/asyncfsm.h>

namespace lwiot
{
	AsyncFsm::AsyncFsm(const stl::String& name) : Base(), _executor(name)
	{
	}

	AsyncFsm::AsyncFsm(AsyncFsm &&other) noexcept : Base(stl::forward<AsyncFsm>(other)), _executor(stl::move(other._executor))
	{
	}

	AsyncFsm::~AsyncFsm()
	{
		this->_transition_q.signal();
		this->stop();
	}

	void AsyncFsm::run()
	{
		while(this->running()) {
			UniqueLock<LockType> lock(this->_lock);

			if(this->_events.empty())
				this->_transition_q.wait(lock, Timeout);

			if(this->_events.empty())
				continue;

			Base::transition();
		}
	}

	bool AsyncFsm::stop()
	{
		if(!this->running()) {
			this->_executor.join();
			this->_executor.stop();

			return true;
		}

		Base::stop(true);

		this->_executor.join();
		this->_executor.stop();

		return true;
	}

	void AsyncFsm::start()
	{
		Base::start(true);
		this->_executor.start([&]() { this->run(); });
	}

	void AsyncFsm::halt()
	{
		Base::halt();

		this->_transition_q.signal();
		this->_executor.join();
	}

	AsyncFsm::FsmStatus AsyncFsm::raiseFromIrq(AsyncFsm::FsmEventType &&event, AsyncFsm::SignalPointer signal)
	{
		enter_critical();
		auto rv = Base::raise(stl::forward<FsmEventType>(event), stl::move(signal));
		exit_critical();

		this->_transition_q.signalFromIrq();
		return rv ? this->status() : FsmStatus::StateUnchanged;
	}

	AsyncFsm &AsyncFsm::operator=(AsyncFsm &&other) noexcept
	{
		UniqueLock<LockType> lock(this->_lock);

		this->move(other);
		this->_executor = stl::move(other._executor);
		this->_transition_q = stl::move(other._transition_q);

		return *this;
	}
}
