/*
 * Asynchonous FSM header file.
 *
 * @author Michel Megens
 * @email  michel@michelmegens.net
 */

/**
 * @file asyncfsm.h Asynchronous FSM.
 */

#pragma once

#include <lwiot.h>

#include <lwiot/function.h>
#include <lwiot/stl/string.h>

#include <lwiot/detail/fsm_policy.h>
#include <lwiot/detail/fsm_base.h>

namespace lwiot
{
	/**
	 * @brief Asyncronous FSM.
	 * @see FsmBase
	 * @see FSM
	 *
	 * This FSM implements a threading model in order to execute the FSM in the background.
	 */
	class AsyncFsm : public detail_fsm::FsmBase<bool(stl::SharedPointer<Signal>),
			detail_fsm::MultiThreadingPolicy, detail_fsm::Watchdog> {
	public:
		typedef Signal SignalType; //!< Signal type definition.
		typedef stl::SharedPointer<SignalType> SignalPointer; //!< Handler argument type definition.

	private:
		typedef detail_fsm::FsmBase<bool(stl::SharedPointer<SignalType>), detail_fsm::MultiThreadingPolicy, detail_fsm::Watchdog> Base;
		typedef typename Base::Threading::Thread ThreadType;

	public:
		typedef detail_fsm::FsmStatus FsmStatus; //!< FSM status type.
		typedef Base::FsmEventType FsmEventType; //!< FSM event type (alphabet symbol).

		static_assert(Base::Threaded, "Async FSM is not threaded!");

		/**
		 * @brief Create new FSM object.
		 * @param name Thread name. Defaults to "fsm".
		 */
		explicit AsyncFsm(const stl::String& name = "fsm");

		/**
		 * @brief Move construct a FSM object.
		 * @param other FSM object to move.
		 */
		AsyncFsm(AsyncFsm&& other) noexcept ;

		/**
		 * @brief Destroy a FSM object.
		 */
		~AsyncFsm() override;

		explicit AsyncFsm(const AsyncFsm& other) = delete;
		AsyncFsm& operator=(const AsyncFsm& other) = delete;

		/**
		 * @brief Assign an FSM using move semantics.
		 * @param other FSM object to move.
		 * @return The current FSM object.
		 */
		AsyncFsm& operator=(AsyncFsm&& other) noexcept;

		using Base::transition;

		/**
		 * @brief Raise an event
		 * @tparam T FSM signal / handler argument type.
		 * @param event Alphabet symbol.
		 * @param signal FSM signal / handler argument.
		 * @return FSM
		 *
		 * This method allows external party's to influence the FSM using the FSM's alphabet.
		 */
		template <typename T>
		FsmStatus raise(FsmEventType&& event, T&& signal)
		{
			auto shared = stl::MakeShared<T>(stl::forward<T>(signal));
			auto rv = Base::raise(stl::forward<FsmEventType>(event),
			        stl::move(stl::static_pointer_cast<SignalType>(shared)));

			this->m_transition_q.signal();
			return rv ? this->status() : FsmStatus::StateUnchanged;
		}

		/**
		 * @brief Raise an event from IRQ context.
		 * @tparam T FSM signal / handler argument type.
		 * @param event Alphabet symbol.
		 * @param signal FSM signal / handler argument.
		 * @return True or false based on whether or not the FSM accepts \p event.
		 *
		 * This method allows external party's to influence the FSM using the FSM's alphabet.
		 */
		template <typename T>
		FsmStatus raiseFroMIrq(FsmStateIdType&& event, T&& signal)
		{
			auto ptr = stl::MakeShared<T>(stl::forward<T>(signal));
			enter_critical();
			auto rv = Base::raise(stl::forward<FsmEventType>(event), stl::move(ptr));
			exit_critical();

			this->m_transition_q.signalFromIrq();
			return rv ? this->status() : FsmStatus::StateUnchanged;
		}

		/**
		 * @brief Raise an event from IRQ context.
		 * @param event Alphabet symbol.
		 * @param signal FSM signal / handler argument.
		 * @return True or false based on whether or not the FSM accepts \p event.
		 *
		 * This method allows external party's to influence the FSM using the FSM's alphabet.
		 */
		FsmStatus raiseFromIrq(FsmEventType&& event, SignalPointer signal);

		/**
		 * @brief Raise an event from IRQ context.
		 * @tparam T Signal type.
		 * @param event Alphabet symbol.
		 * @param signal FSM signal / handler argument.
		 * @return FSM
		 *
		 * This method allows external party's to influence the FSM using the FSM's alphabet.
		 */
		template <typename T>
		FsmStatus transition(FsmEventType&& event, T&& signal)
		{
			auto shared = stl::MakeShared<T>(stl::forward<T>(signal));
			auto rv = Base::transition(stl::forward<FsmEventType>(event),
			        stl::move(stl::static_pointer_cast<SignalType>(shared)));

			this->m_transition_q.signal();
			return rv ? this->status() : FsmStatus::StateUnchanged;
		}

		using Base::start;
		using Base::stop;
		using Base::addTransition;

		/**
		 * @brief Add a transition to to the state machine.
		 * @param state Origin state.
		 * @param event Event / alphabet symbol.
		 * @param next Target state.
		 * @return True or false depending on whether or not a transition was created and added to the FSM.
		 */
		bool addTransition(const StateType& state, FsmEventType event, const StateType& next);

		/**
		 * @brief Add a transition to to the state machine.
		 * @tparam Func Guard functor type.
		 * @param state Origin state.
		 * @param event Event / alphabet symbol.
		 * @param next Target state.
		 * @param func Guard functor.
		 * @return True or false depending on whether or not a transition was created and added to the FSM.
		 */
		template <typename Func>
		bool addTransition(const StateType& state, FsmEventType event, const StateType& next, Func&& func)
		{
			return Base::addTransition(state.id(), stl::move(event), next.id(), stl::forward<Func>(func));
		}

		/**
		 * @brief Start the FSM.
		 * @note This method will start a new thread.
		 */
		virtual void start();
		/**
		 * @brief Stop an FSM.
		 * @return True or false based on whether or not the FSM has been stopped succesfully or not.
		 */
		virtual bool stop();

		/**
		 * @brief Force stop an FSM.
		 * @note This method will not wait for the FSM to enter a valid stop state.
		 */
		void halt() override;

	protected:
		/**
		 * @brief FSM runner.
		 */
		void run() override ;

	private:
		ThreadType m_executor;
		Base::EventType m_transition_q;
	};
}
