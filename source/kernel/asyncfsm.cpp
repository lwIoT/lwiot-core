/*
 * Asynchronous FSM implementation.
 *
 * @author Michel Megens
 * @email  michel@michelmegens.net
 */

#include <lwiot.h>

#include <lwiot/kernel/asyncfsm.h>

#include <lwiot/stl/string.h>
#include <lwiot/stl/bind.h>

namespace lwiot
{
	AsyncFsm::AsyncFsm(const stl::String& name) : Base(), m_executor(name)
	{
	}

	AsyncFsm::AsyncFsm(AsyncFsm &&other) noexcept : Base(stl::forward<AsyncFsm>(other)), m_executor(stl::move(other.m_executor))
	{
	}

	AsyncFsm::~AsyncFsm()
	{
		this->m_transition_q.signal();
		this->stop();
	}

	void AsyncFsm::run()
	{
		while(this->running()) {
			UniqueLock<LockType> lock(this->m_lock);

			if(this->m_events.empty())
				this->m_transition_q.wait(lock, Timeout);

			if(this->m_events.empty())
				continue;

			Base::transition();
		}
	}

	bool AsyncFsm::stop()
	{
		if(!this->running()) {
			this->m_executor.join();
			this->m_executor.stop();

			return true;
		}

		Base::stop(true);

		this->m_executor.join();
		this->m_executor.stop();

		return true;
	}

	void AsyncFsm::start()
	{
		Base::start(true);
		this->m_executor.start(stl::bind(&AsyncFsm::run, this));
	}

	void AsyncFsm::halt()
	{
		Base::halt();

		this->m_transition_q.signal();
		this->m_executor.join();
	}

	AsyncFsm::FsmStatus AsyncFsm::raiseFromIrq(AsyncFsm::FsmEventType &&event, AsyncFsm::SignalPointer signal)
	{
		enter_critical();
		auto rv = Base::raise(stl::forward<FsmEventType>(event), stl::move(signal));
		exit_critical();

		this->m_transition_q.signalFromIrq();
		return rv ? this->status() : FsmStatus::StateUnchanged;
	}

	AsyncFsm &AsyncFsm::operator=(AsyncFsm &&other) noexcept
	{
		UniqueLock<LockType> lock(this->m_lock);

		this->move(other);
		this->m_executor = stl::move(other.m_executor);
		this->m_transition_q = stl::move(other.m_transition_q);

		return *this;
	}

	bool AsyncFsm::addTransition(const StateType& state, FsmEventType event, const StateType& next)
	{
		return Base::addTransition(state.id(), stl::move(event), next.id());
	}
}
