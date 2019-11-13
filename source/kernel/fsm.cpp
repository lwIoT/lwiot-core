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
	FSM::FSM() : Base(), _blocking(false)
	{
	}

	FSM::FSM(const FSM &fsm) : Base(fsm), _blocking(fsm._blocking)
	{ }

	FSM::FSM(FSM &&other) noexcept : Base(stl::forward<FSM>(other)), _blocking(other._blocking)
	{
	}

	FSM &FSM::operator=(const FSM &other)
	{
		this->copy(other);
		this->_blocking = other._blocking;

		return *this;
	}

	FSM &FSM::operator=(FSM &&other) noexcept
	{
		this->move(other);
		this->_blocking = other._blocking;

		return *this;
	}

	void FSM::run()
	{
		if(!this->running())
			return;

		if(this->_blocking) {
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
		if(this->_events.empty())
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
		UniqueLock<Base::LockType> lock(this->_lock);
		this->_blocking = blocking;
	}

	bool FSM::blocking() const
	{
		UniqueLock<Base::LockType> lock(this->_lock);
		return this->_blocking;
	}
}
