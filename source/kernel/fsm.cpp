/*
 * Finite state machine header.
 *
 * @author Michel Megens
 * @email  michel@michelmegens.net
 */

#include <lwiot/detail/fsm_base.h>
#include <lwiot/kernel/fsm.h>

namespace lwiot
{
	FSM::FSM() : Base(), m_blocking(false)
	{
	}

	FSM::FSM(const FSM &fsm) : Base(fsm), m_blocking(fsm.m_blocking)
	{ }

	FSM::FSM(FSM &&other) noexcept : Base(stl::forward<FSM>(other)), m_blocking(other.m_blocking)
	{
	}

	FSM &FSM::operator=(const FSM &other)
	{
		this->copy(other);
		this->m_blocking = other.m_blocking;

		return *this;
	}

	FSM &FSM::operator=(FSM &&other) noexcept
	{
		this->move(other);
		this->m_blocking = other.m_blocking;

		return *this;
	}

	void FSM::run()
	{
		if(!this->running())
			return;

		if(this->m_blocking) {
			do {
				this->cycle();
			} while(this->running());
		} else {
			this->cycle();
		}
	}

	void FSM::start()
	{
		Base::start(true);
	}

	bool FSM::stop()
	{
		for(auto i = 0; i < MaxRetryCount; i++) {
			if(Base::stop(true))
				return true;
		}

		return false;
	}

	void FSM::cycle()
	{
		if(this->m_events.empty())
			return;

		this->transition();
	}

	bool FSM::raiseFromIrq(FSM::FsmEventType &&event, FSM::SignalPointer &&signal)
	{
		enter_critical();
		auto rv = this->transition(stl::forward<FsmEventType>(event), stl::forward<SignalPointer>(signal));
		exit_critical();

		return rv;
	}

	void FSM::block(bool blocking)
	{
		UniqueLock<Base::LockType> lock(this->m_lock);
		this->m_blocking = blocking;
	}

	bool FSM::blocking() const
	{
		UniqueLock<Base::LockType> lock(this->m_lock);
		return this->m_blocking;
	}

	bool FSM::addTransition(const StateType& state, FsmEventType event, const StateType& next)
	{
		return Base::addTransition(state.id(), stl::move(event), next.id());
	}
}
