/*
 * Finite state machine header.
 *
 * @author Michel Megens
 * @email  michel@michelmegens.net
 */

/**
 * @file fsm.h Synchronous FSM header.
 */

#pragma once

#include <lwiot/detail/fsm_policy.h>
#include <lwiot/detail/fsm_base.h>

namespace lwiot
{
	/**
	 * @brief Synchronous FSM implementation.
	 */
	class FSM : public detail_fsm::FsmBase<bool(stl::SharedPointer<Signal>),
			detail_fsm::SingleThreadingPolicy, detail_fsm::Watchdog> {
		typedef detail_fsm::FsmBase<bool(stl::SharedPointer<Signal>), detail_fsm::SingleThreadingPolicy, detail_fsm::Watchdog> Base;

		static constexpr int MaxRetryCount = 3;

	public:
		typedef Signal SignalType; //!< FSM sigal type.
		typedef stl::SharedPointer<SignalType> SignalPointer; //!< Signal pointer type.
		typedef detail_fsm::FsmStatus FsmStatus; //!< FSM status type.
		typedef Base::FsmEventType FsmEventType; //!< FSM event type.

		/**
		 * @brief Create a new FSM object.
		 */
		explicit FSM();

		/**
		 * @brief Copy construct a FSM object.
		 * @param fsm FSM object to copy.
		 */
		FSM(const FSM& fsm);

		/**
		 * @brief Move construct a FSM object.
		 * @param other FSM object to move construct.
		 */
		FSM(FSM&& other) noexcept;

		/**
		 * @brief Copy assignment operator.
		 * @param other Object to copy.
		 * @return This FSM object.
		 */
		FSM& operator=(const FSM& other);

		/**
		 * @brief Assign a FSM object using move semantics.
		 * @param other FSM object to move.
		 * @return This FSM object.
		 */
		FSM& operator=(FSM&& other) noexcept;

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
			auto casted = stl::static_pointer_cast<Signal>(stl::move(shared));

			auto rv = Base::raise(stl::forward<FsmEventType>(event), stl::move(casted));
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
		bool raiseFromIrq(FsmEventType&& event, SignalPointer&& signal);

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
		FsmStatus raiseFromIrq(FsmEventType&& event, T&& signal)
		{
			auto ptr = stl::MakeShared<T>(stl::forward<T>(signal));

			enter_critical();
			auto rv = this->transition(stl::forward<FsmEventType>(event), stl::forward<SignalPointer>(ptr));
			exit_critical();

			return rv;
		}

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
			auto casted = stl::static_pointer_cast<SignalType>(stl::move(shared));

			auto rv = Base::transition(stl::forward<FsmEventType>(event), stl::move(casted));
			return rv ? this->status() : FsmStatus::StateUnchanged;
		}

		/**
		 * @brief Execute the FSM.
		 *
		 * This method may or may not block based on whether or not this FSM has been configured as blocking or not.
		 */
		void run() override;

		/**
		 * @brief Start the FSM.
		 */
		void start();

		/**
		 * @brief Stop the FSM.
		 * @return True or false based on whether or not the FSM has succesfully stopped or not.
		 */
		bool stop();

		/**
		 * @brief Set the blocking state of the FSM.
		 * @param blocking Blocking indicator.
		 * @see FSM::blocking FSM::run
		 */
		void block(bool blocking);

		/**
		 * @brief Getter for the blocking indicator.
		 * @see FSM::run
		 * @see FSM::block
		 * @return True or false based on whether or not this FSM's FSM::run method will block or not.
		 */
		bool blocking() const;


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

	protected:
		using Base::transition;

	private:
		bool m_blocking;

		/* METHODS */
		void cycle();
	};
}
