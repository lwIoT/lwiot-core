/*
 * Finite State Machine base class definition.
 *
 * @author Michel Megens
 * @email  michel@michelmegens.net
 */

/**
 * @file fsm_base.h FSM base header.
 */

#pragma once

#include "fsm_policy.h"

#include <lwiot/stl/tuple.h>
#include <lwiot/stl/pair.h>
#include <lwiot/stl/referencewrapper.h>
#include <lwiot/stl/sharedpointer.h>
#include <lwiot/stl/array.h>

#include <lwiot/kernel/uniquelock.h>

#include <lwiot/traits/integralconstant.h>
#include <lwiot/traits/isintegral.h>
#include <lwiot/traits/ismoveconstructible.h>
#include <lwiot/traits/iscopyconstructible.h>
#include <lwiot/traits/isconvirtible.h>
#include <lwiot/traits/issame.h>

namespace lwiot
{
	namespace detail_fsm
	{
		template<size_t... Indices>
		struct IndexSequence {
			using Type = IndexSequence<Indices...>;

			constexpr size_t size() const noexcept
			{
				return sizeof...(Indices);
			}
		};

		template<size_t idx, size_t... Indices>
		struct MakeIndexSequence : MakeIndexSequence<idx - 1, idx - 1, Indices...> {
		};

		template<size_t... Indices>
		struct MakeIndexSequence<0, Indices...> {
			using Type = IndexSequence<Indices...>;

			static constexpr size_t size() noexcept
			{
				return sizeof...(Indices);
			}
		};

		/**
		 * @brief Check if the FSM policy defines a threading model.
		 * @tparam T Policy type.
		 */
		template<typename T>
		struct HasThreading {
		public:
			template<typename U>
			static constexpr traits::TrueType test(typename U::Threading *)
			{
				return {};
			}

			template<typename>
			static constexpr traits::FalseType test(...)
			{
				return {};
			}

		private:
			enum data {
				value = !!decltype(test<T>(0))()
			};

		public:
			static constexpr bool Value = data::value; //!< Indicator if \p T defines a threading model.
		};

		/**
		 * @brief Select a threading policy.
		 * @tparam T Policy type.
		 * @tparam threading Indicator if \p T has a threading model.
		 * @see HasThreading
		 */
		template<typename T, bool threading>
		struct SelectThreadingPolicy;

		template<typename T>
		struct SelectThreadingPolicy<T, true> {
			typedef typename T::Threading Type;
		};

		template<typename T>
		struct SelectThreadingPolicy<T, false> {
			typedef SingleThreading Type;
		};

		template<typename... T>
		struct TemplateForAll;

		template<>
		struct TemplateForAll<> : public traits::TrueType {
		};

		template<typename... T>
		struct TemplateForAll<traits::FalseType, T...> : public traits::FalseType {
		};

		template<typename... T>
		struct TemplateForAll<traits::TrueType, T...> : public TemplateForAll<T...>::type {
		};

		typedef uint32_t FsmStateId; //!< FSM state ID type.

		/**
		 * FSM base helper. The helper checks the validity of the argument types. All arguments should be
		 * both move and copy constructible.
		 *
		 * @tparam W Watchdog type.
		 * @tparam Args Handler arguments.
		 */
		template<typename W, typename... Args>
		struct FsmBase_helper {
			/**
			 * Create a new FsmBase_helper object and construct a default watchdog object.
			 */
			explicit FsmBase_helper() : _watchdog()
			{
			}

			FsmBase_helper(FsmBase_helper&& other) noexcept = default;
			FsmBase_helper(const FsmBase_helper& other) = default;

			FsmBase_helper& operator=(FsmBase_helper&& other) noexcept = default;
			FsmBase_helper& operator=(const FsmBase_helper& other) = default;

			/**
			 * Create a new FsmBase_helper object and construct a default watchdog object, setting the WDT
			 * timeout to \p timeout.
			 *
			 * @param timeout WDT timeout.
			 */
			explicit FsmBase_helper(time_t timeout) : _watchdog()
			{
				this->_watchdog.enable(timeout);
			}

		private:
			typedef W WatchdogType; //!< FSM watchdog type definition.

			/**
			 * Check if all arguments are move constructible.
			 *
			 * @return True or false.
			 */
			static constexpr bool IsMoveConstructible()
			{
				return TemplateForAll<typename traits::IsMoveConstructible<Args>::type...>::value;
			}

			/**
			 * Check if all arguments are copy constructible.
			 *
			 * @return True or false.
			 */
			static constexpr bool IsCopyConstructible()
			{
				return TemplateForAll<typename traits::IsCopyConstructible<Args>::type...>::value;
			}

			/**
			 * Check if the Watchdog type is constructible.
			 *
			 * @return True or false.
			 */
			static constexpr bool CanCreateWatchdog()
			{
				return traits::IsConstructible<WatchdogType>::value;
			}


		protected:
			WatchdogType _watchdog; //!< FSM watchdog.

			static_assert(FsmBase_helper::IsCopyConstructible(), "Arguments must be copy constructible!");
			static_assert(FsmBase_helper::IsMoveConstructible(), "Arguments must be move constructible!");
			static_assert(FsmBase_helper::CanCreateWatchdog(), "Unable to construct watchdog type!");
		};

		/**
		 * @brief FSM status.
		 */
		enum class FsmStatus {
			StateUnchanged, //!< No transition has taken place.
			StateChanged, //!< The FSM transitioned from one state to another.
			Fault, //!< Fatal error indicator.
			Error, //!< Error indicator. The error state has been executed.
			Stopped, //!< FSM is not running.
			Running //!< FSM is running.
		};

		/**
		 * Transition type definition.
		 * @tparam P Policy type.
		 * @tparam Args Handler argument types.
		 */
		template<typename P, typename... Args>
		class Transition {
		public:
			typedef P PolicyType; //!< Policy type definition.
			typedef typename P::FsmEvent FsmEventType; //!< FSM event type definition.
			typedef Function<bool(Args...)> GuardHandlerType; //!< Guard handler type.
			typedef FsmStateId FsmStateIdType; //!< State ID type.

			/**
			 * @brief Create a new transition object.
			 */
			explicit Transition() : _handler(), _event(), _next()
			{
			}

			/**
			 * @brief Create a new transition.
			 * @param event Alphabet symbol that triggers this transition.
			 * @param next State this transition transitions into.
			 * @param handler Guard handler.
			 */
			explicit Transition(const FsmEventType &event, const FsmStateIdType &next, const GuardHandlerType &handler) :
				_handler(handler), _event(event), _next(next)
			{
			}

			/**
			 * @brief Create a new transition.
			 * @param event Alphabet symbol that triggers this transition.
			 * @param next State this transition transitions into.
			 */
			explicit Transition(const FsmEventType &event, const FsmStateIdType &next) :
				_handler(), _event(event), _next(next)
			{
			}

			/**
			 * @brief Execute the guard handler.
			 * @param args Guard handler arguments.
			 * @return True or false based on whether or not the guard handler succeeded or not.
			 */
			bool guard(Args &&... args)
			{
				return this->_handler(stl::forward<Args>(args)...);
			}

			/**
			 * @brief Check if this transition has a guard handler.
			 * @return True or false based on whether or not this state has a guard handler.
			 */
			bool hasGuard() const
			{
				return static_cast<bool>(this->_handler.valid());
			}

			/**
			 * @brief Getter for the event (alphabet symbol).
			 * @return The alphabet symbol tied to this transition.
			 */
			const FsmEventType &event() const
			{
				return this->_event;
			}

			/**
			 * @brief Equality operator.
			 * @param eventId Alphabet symbol to test against.
			 * @return True or false based on whether or not the transition and \p eventId match.
			 */
			bool operator==(const FsmEventType &eventId) const
			{
				return this->_event == eventId;
			}

			/**
			 * @brief Inequality operator.
			 * @param eventId Alphabet symbol to test against.
			 * @return The inverse of the equality operator.
			 */
			bool operator!=(const FsmEventType &eventId) const
			{
				return this->_event != eventId;
			}

			/**
			 * @brief Set the guard handler.
			 * @tparam Func Guard handler type.
			 * @param func Guard handler to assign to this transition.
			 */
			template<typename Func>
			void setGuard(Func &&func)
			{
				this->_handler = stl::forward<Func>(func);
			}

			/**
			 * @brief Set the guard handler.
			 * @param guard Guard handler to set.
			 */
			void setGuard(const GuardHandlerType &guard)
			{
				this->_handler = guard;
			}

			/**
			 * @brief Set the alphabet symbol of this transition.
			 * @param event Alphabet symbol.
			 */
			void setEvent(const FsmEventType &event)
			{
				this->_event = event;
			}

			/**
			 * @brief Set the state ID of the state this transition transitions into.
			 * @param next The state this transition leads to.
			 */
			void setNext(const FsmStateIdType &next)
			{
				this->_next = next;
			}

			/**
			 * @brief Getter for the continuation state.
			 * @return The continuation state of this transition.
			 */
			const FsmStateIdType &next() const
			{
				return this->_next;
			}

		private:
			GuardHandlerType _handler; //!< Guard handler.
			FsmEventType _event; //!< Alphabet symbol tied to the transition.
			FsmStateIdType _next; //!< Continuation state.
		};

		/**
		 * @brief FSM state definition.
		 * @tparam P Policy type.
		 * @tparam H Handler type.
		 * @tparam R Return type.
		 * @tparam Args Handler arguments.
		 */
		template<typename P, typename H, typename R, typename... Args>
		class State {
			typedef P PolicyType; //!< Policy type.
			typedef H HandlerType; //!< State handler type definition.
			typedef Transition<P, Args...> TransitionType; //!< Transition type.
			typedef typename PolicyType::FsmEvent FsmEventType; //!< FSM event type.
			typedef FsmStateId FsmStateIdType; //!< State ID type.

			/**
			 * @brief Dynamic array type definition.
			 * @tparam T value type.
			 */
			template <typename T>
			using ArrayList = typename PolicyType::template ArrayList<T>;

		public:
			/**
			 * @brief Create a new state object.
			 */
			explicit State() : _id(State::generateFsmStateId()), _parent()
			{
			}

			/**
			 * @brief Create a new state with \p parent as a parent state.
			 * @param parent Parent state.
			 */
			explicit State(const FsmStateIdType parent) : _id(generateFsmStateId()), _parent(parent)
			{
			}

			/**
			 * @brief Invoke the state action.
			 * @tparam ReturnType StateHandler return type.
			 * @param args StateHandler arguments.
			 * @return State return type. Defined by the implementor of FsmBase.
			 */
			template<typename ReturnType = R>
			inline traits::EnableIf_t<traits::Not<traits::IsSame<void, ReturnType>>::value, bool>
			action(Args &&... args)
			{
				return this->_action(stl::forward<Args>(args)...);
			}

			/**
			 * @brief Invoke the state action.
			 * @tparam ReturnType State return type.
			 * @param args StateHandler arguments.
			 * @return Always true.
			 */
			template<typename ReturnType = R>
			inline traits::EnableIf_t<traits::IsSame<void, ReturnType>::value, bool> action(Args &&... args)
			{
				this->_action(stl::forward<Args>(args)...);
				return true;
			}

			/**
			 * @brief Add a transition to the state.
			 * @param transition Transition to add.
			 */
			inline void addTransition(const TransitionType &transition)
			{
				this->_transitions.push_back(transition);
			}

			/**
			 * @brief Add a transition to the state.
			 * @param transition Transition to add using move semantics.
			 */
			inline void addTransition(TransitionType &&transition)
			{
				this->_transitions.push_back(stl::forward<TransitionType>(transition));
			}

			/**
			 * @brief Construct a new transition and add it to the state.
			 * @tparam Func GuardHandler type.
			 * @param event Event ID.
			 * @param next State the transition transitions to.
			 * @param guard Transition guard.
			 */
			template<typename Func>
			inline void addTransition(const FsmEventType &event, const FsmStateIdType &next, Func &&guard)
			{
				TransitionType t(event, next);

				t.setGuard(stl::forward<Func>(guard));
				this->_transitions.push_back(stl::move(t));
			}

			/**
			 * @brief Construct a new transition and add it to the state.
			 * @param event Event ID.
			 * @param next State the transition transitions to.
			 * @param guard Transition guard.
			 */
			inline void addTransition(const FsmEventType &event, const FsmStateIdType &next,
			                          const typename TransitionType::GuardHandlerType &guard)
			{
				TransitionType t(event, next);

				t.setGuard(guard);
				this->_transitions.push_back(stl::move(t));
			}

			/**
			 * @brief Add a transition to the state.
			 * @param event Event ID.
			 * @param next State the transition transitions to.
			 */
			inline void addTransition(const FsmEventType &event, const FsmStateIdType &next)
			{
				TransitionType t(event, next);

				this->_transitions.push_back(stl::move(t));
			}

			/**
			 * @brief Set the state ID.
			 * @param id State ID to set.
			 */
			void setId(const FsmStateIdType &id)
			{
				this->_id = id;
			}

			/**
			 * @brief Set a parent state by ID.
			 * @param id ID of the parent state.
			 */
			void setParent(const FsmStateIdType &id)
			{
				this->_parent = id;
			}

			/**
			 * @brief Set a parent state.
			 * @param state State to set as parent.
			 */
			void setParent(const State &state)
			{
				this->_parent = state._id;
			}

			/**
			 * @brief State ID getter.
			 * @return The state ID.
			 */
			const FsmStateIdType &id() const
			{
				return this->_id;
			}

			/**
			 * @brief Parent state ID getter.
			 * @return The ID of the parent state.
			 * @note The return value of this function is undefined if State::hasParent returns false.
			 */
			const FsmStateIdType &parent() const
			{
				return this->_parent;
			}

			/**
			 * @brief Transition table getter.
			 * @return Dynamic array of transitions.
			 */
			const ArrayList<TransitionType> &transitions() const
			{
				return this->_transitions;
			}

			/**
			 * @brief StateHandler setter.
			 * @param handler State handler to bind to this state.
			 */
			void setAction(const HandlerType &handler)
			{
				this->_action = handler;
			}

			/**
			 * @brief StateHandler setter.
			 * @tparam Action StateHandler type.
			 * @param act Handler to set.
			 */
			template<typename Action>
			void setAction(Action &&act)
			{
				this->_action = stl::forward<Action>(act);
			}

			/**
			 * @brief Check if this state has a valid handler.
			 * @return True or false based on whether or not this state has a valid handler.
			 */
			bool hasAction() const
			{
				return static_cast<bool>(this->_action);
			}

			/**
			 * @brief Check if this state has a valid parent object.
			 * @return True or false based on whether or not this state has a valid parent object.
			 */
			bool hasParent() const
			{
				return static_cast<bool>(this->_parent);
			}

		private:
			FsmStateIdType _id; //!< State ID.
			FsmStateIdType _parent; //!< Parent state ID.
			ArrayList<TransitionType> _transitions; //!< Transition table.

			HandlerType _action; //!< State handler object.

			/* METHODS */

			/**
			 * @brief Generate a unique state ID.
			 * @return A unique state ID type.
			 */
			static FsmStateIdType generateFsmStateId()
			{
				FsmStateIdType result = 0UL;

				for(auto num = 0UL; num < sizeof(FsmStateIdType); num++) {
					auto r = lwiot::random();
					r %= 1U << BITS_PER_BYTE;

					result |= r << (num * BITS_PER_BYTE);
				}

				return result;
			}
		};

#ifdef DOXYGEN
		/**
		 * @brief FSM base class for various FSM implementations.
		 *
		 * A finite state machine (FSM) is a mathematical model of computation. It is an abstract
		 * machine that can only be in one of a finite number of states. A transition from one state
		 * to the next can either be initiated by the state execution itself, or by external input. A
		 * FSM is defined by defined by a quintuple \f$(\sum,S,s_{0},\delta, F)\f$, where:
		 *
		 *  * \f$\sum\f$ is the input alphabet, a finite non-empty set of symbols
		 *  * \f$S\f$ is a finite non-empty set of states
		 *  * \f$s_{0}\f$ is the initial state (member of S)
		 *  * \f$\delta\f$ is the transition function: \f$\delta : S \times \sum \rightarrow S\f$
		 *  * \f$F\f$ is a finite, possibly empty, set of final states (subset of S)
		 *
		 * @see FsmBase_helper
		 * @tparam Handler State handler type.
		 * @tparam Policy Policy type.
		 * @tparam Watchdog Watchdog type.
		 */
		template <typename Handler, typename Policy, typename Watchdog>
		class FsmBase : public FsmBase_helper<W, Args...>
#else

		template<typename Handler, typename Policy, typename Watchdog>
		class FsmBase;

		template<typename P, typename W, typename R, typename... Args>
		class FsmBase<R(Args...), P, W> : public FsmBase_helper<W, Args...>
#endif
		{
			typedef FsmBase_helper<W, Args...> Base; //!< Base class type.
			typedef FsmBase<R(Args...), P, W> ThisClass; //!< This class definition.

		protected:
			/**
			 * @brief Set to true if a threading model is present on the PolicyType.
			 */
			static constexpr bool Threaded = HasThreading<P>::Value;

		public:
			typedef R ReturnType; //!< Return type definition.
			typedef P PolicyType; //!< Policy type definition.

			typedef typename SelectThreadingPolicy<PolicyType, Threaded>::Type Threading; //!< Threading model definition.
			typedef Function<ReturnType(Args...)> HandlerType; //!< State handler.
			typedef State<P, HandlerType, ReturnType, Args...> StateType; //!< State type.
			typedef typename Threading::Lock LockType; //!< Mutex type.
			typedef typename Threading::Event EventType; //!< Waiting queue type.
			typedef typename PolicyType::FsmEvent FsmEventType; //!< FSM event type.
			typedef Transition<P, Args...> TransitionType; //!< Transition type.
			typedef FsmStateId FsmStateIdType; //!< State ID type.

			/**
			 * @brief Queue type.
			 * @tparam T Encapsulated type.
			 */
			template<typename T>
			using QueueType = typename PolicyType::template Queue<T>;
			template<typename T>

			/**
			 * @brief Dynamic array type.
			 * @tparam T Encapsulated type.
			 */
			using ArrayListType = typename PolicyType::template ArrayList<T>;
			/**
			 * @brief Map type.
			 * @tparam K Key type.
			 * @tparam V Value type.
			 */
			template<typename K, typename V>
			using MapType = typename PolicyType::template Map<K, V>;
			/**
			 * @brief Unordered set type.
			 * @tparam T Encapsulated type.
			 */
			template<typename T>
			using SetType = typename PolicyType::template Set<T>;

		private:
			/// @name Type check methods
			/// @{

			/**
			 * @brief Check if \p _Tp is an integral type.
			 * @tparam _Tp Type to check.
			 * @return True or false based on whether \p _Tp is an integral type.
			 */
			template <typename _Tp>
			static constexpr bool IsIntegralType()
			{
				return traits::IsIntegral<_Tp>::Value;
			}

			/**
			 * @brief Check if \_Tp is void.
			 * @tparam _Tp Type to check.
			 * @return True or false based on whether \p _Tp is void.
			 *
			 * This method uses traits::IsSame to check \p _Tp.
			 */
			template<typename _Tp>
			constexpr static bool IsVoid()
			{
				return traits::IsSame<_Tp, void>::value;
			}

			/**
			 * @brief Check if \p _Tp is void or bool.
			 * @tparam _Tp Type to check.
			 * @return True or false based on whether \p _Tp is void or bool or not.
			 */
			template<typename _Tp>
			constexpr static bool IsVoidOrBool()
			{
				return IsVoid<_Tp>() || IsBoolable<_Tp>();
			}

			/**
			 * @brief Check if \p _Tp is convirtable to bool.
			 * @tparam _Tp Type to check.
			 * @return  True or false based on whether \p is convirtable to bool or not.
			 */
			template<typename _Tp>
			constexpr static bool IsBoolable()
			{
				constexpr auto is_bool = traits::IsSame<_Tp, bool>::value;
				constexpr auto is_conv = traits::IsConvirtable<_Tp, bool>::value;

				return is_bool || is_conv;
			}

			/// @}

			static_assert(IsIntegralType<FsmStateIdType>(), "State ID should be an integral constant!");
			static_assert(IsIntegralType<FsmEventType>(), "FSM Event type should be an integral constant!");
			static_assert(IsVoidOrBool<ReturnType>(), "Return value should be bool or void!");
			static_assert(IsBoolable<FsmEventType>(), "EventType should be convirtable to bool!");
			static_assert(IsBoolable<FsmStateIdType>(), "EventIdType should be convirtable to bool!");

		protected:
			/**
			 * @brief Argument wrapper type.
			 * @see stl::Tuple
			 *
			 * The state handler argument values are wrapper in the \p ArgumentWrapper while they are stored on
			 * the event queue.
			 */
			using ArgumentWrapper = stl::Tuple<typename traits::RemoveCv<typename traits::RemoveCv<Args>::type>::type...>;

		public:

			/**
			 * @brief Create a new FsmBase object.
			 * @param tmo Timeout for the watchdog timer.
			 */
			explicit FsmBase(time_t tmo = 2000);

			/**
			 * @brief Create a new FsmBase object.
			 * @param silent Indicator if log messages should be suppressed or not.
			 * @param tmo Timeout for the watchdog timer.
			 */
			explicit FsmBase(bool silent, time_t tmo = 2000);

			/**
			 * @brief Copy an FsmBase object.
			 * @param other Object to copy.
			 */
			FsmBase(const FsmBase &other);

			/**
			 * @brief Move construct an FSM base object.
			 * @param other Object to move.
			 */
			FsmBase(FsmBase &&other) noexcept;

			/**
			 * @brief Destroy an FSM base object.
			 */
			virtual ~FsmBase();

			/**
			 * @brief Move assignment operator implementation.
			 *
			 * @param other FSM object to move.
			 * @see move
			 * @return Current object.
			 */
			FsmBase &operator=(FsmBase &&other) noexcept;

			/**
			 * @brief Copy assignment operator implementation.
			 *
			 * @param other FSM to copy.
			 * @return Current object.
			 */
			FsmBase &operator=(const FsmBase &other);

			/**
			 * @brief Swap two FSM's.
			 *
			 * @param a First FSM.
			 * @param b Second FSM.
			 * @see lwiot::stl::swap
			 */
			friend void swap(FsmBase &a, FsmBase &b)
			{
				using lwiot::stl::swap;

				if(&a < &b) {
					a._lock.lock();
					b._lock.lock();
				} else {
					b._lock.lock();
					a._lock.lock();
				}

				swap(a._events, b._events);
				swap(a._states, b._states);
				swap(a._stop_states, b._stop_states);
				swap(a._start_state, b._start_state);
				swap(a._current, b._current);
				swap(a._error_state, b._error_state);
				swap(a._status, b._status);
				swap(a._in_transition, b._in_transition);
				swap(a._logger, b._logger);
				swap(a._silent, b._silent);

				if(&a < &b) {
					b._lock.unlock();
					a._lock.unlock();
				} else {
					a._lock.unlock();
					b._lock.unlock();
				}
			}

			/**
			 * @name State methods
			 * @{
			 */

			/**
			 * @brief Request the current status.
			 * @return Current FSM status.
			 * @see FsmStatus _lock
			 * @note This methods acquires _lock.
			 */
			inline FsmStatus status() const;

			/**
			 * @brief Check if the FSM is currently running.
			 * @return True or false based on whether the FSM is running or not.
			 * @note This methods acquires _lock.
			 */
			inline bool running() const;

			/**
			 * @brief Get the current State.
			 * @return The current FSM state.
			 * @note The return value of this method is undefined if the FSM is not running.
			 */
			stl::SharedPointer<StateType> current() const;

			/**
			 * @brief Check if the FSM accepts a given \p event.
			 *
			 * @param event Event to check.
			 * @return True or false based on whether the FSM accepts \p event.
			 */
			bool accept(const FsmEventType &event) const;

			/**
			 * @brief Check if the FSM is valid.
			 * @return True or false based on whether the FSM is valid or not.
			 * @see FsmStatus
			 */
			bool valid() const;

			/**
			 * @brief Check if the FSM is deterministic.
			 * @return True or false based on whether the FSM is deterministic or not.
			 */
			bool deterministic() const;
			///@}

			/// @name FSM builder methods
			/// @{

			/**
			 * @brief Add a new state.
			 * @param state State to add.
			 * @return A stl::Pair containing the assigned ID and a success indicator.
			 * @note The first stl::Pair value is undefined if the second value of the return stl::Pair is false.
			 */
			stl::Pair<FsmStateIdType, bool> addState(StateType &&state);

			/**
			 * @brief Add a new state.
			 * @param state State to add.
			 * @return A stl::Pair containing the assigned ID and a success indicator.
			 * @note The first stl::Pair value is undefined if the second value of the return stl::Pair is false.
			 */
			stl::Pair<FsmStateIdType, bool> addState(const StateType &state);

			/**
			 * @brief Add a range of states.
			 * @param states Dynamic array of states to add to the FSM.
			 * @return True or false based on whether \p states has been added to the FSM.
			 */
			bool addStates(ArrayListType<StateType> &states);

			/**
			 * @brief Define the start state.
			 * @param id Start state ID.
			 */
			void setStartState(const FsmStateIdType &id);

			/**
			 * @brief Add a stop state.
			 * @param state State ID to add to the set of stop states.
			 * @return True or false based on whether or not \p state has been added to the stop state set.
			 */
			bool addStopState(const FsmStateIdType &state);

			/**
			 * @brief Add a range of stop states.
			 * @param states Array of state IDs to add to the stop state set.
			 * @return True or false based on whether or not \p states have been added to the stop state set.
			 */
			bool addStopStates(const ArrayListType<FsmStateIdType> &states);

			/**
			 * @brief Define the error state.
			 * @param id Error state ID.
			 * @return True or false based on whether or not \p id has been defined as the error state.
			 */
			bool setErrorState(const FsmStateIdType &id);

			/**
			 * @brief Add a new symbol to the alphabet.
			 * @param event Event symbol to add to the alphabet.
			 * @see transition raise
			 * @see _alphabet
			 * @return True or false based on whether or not \p event has been added to the alphabet.
			 */
			bool addAlphabetSymbol(const FsmEventType& event);

			/**
			 * @brief Add a new symbol to the alphabet using move semantics.
			 * @param event Event symbol to add to the alphabet.
			 * @see transition raise
			 * @see _alphabet
			 * @return True or false based on whether or not \p event has been added to the alphabet.
			 */
			bool addAlphabetSymbol(FsmEventType&& event);
			/// @}

			/// @name FSM operation methods
			/// @{
			/**
			 * @brief Force stop the FSM.
			 * @note This method will not wait for the FSM to reach a stop state.
			 */
			virtual void halt();

			/**
			 * @brief Execute the FSM.
			 */
			virtual void run() = 0;

			/**
			 * @brief Transition from one state to another.
			 * @param event Input event, which must be in the alphabet.
			 * @param args Arguments to pass to the state and transition handlers.
			 * @return True or false based on whether or not the FSM accepts \p event.
			 * @note This method should be used by state handlers to invoke a transition to the next state. External
			 *       entity's should use FsmBase::raise to influence the FSM.
			 *
			 * This method invokes a transition. Only a single transition can be queued at any given time, attempting
			 * to queue a second transition will result in this method returning \p false.
			 */
			virtual bool transition(FsmEventType &&event, Args &&... args);

			/**
			 * @brief Raise an FSM event in order to transition to another state.
			 * @param event Input event, which must be in the alphabet.
			 * @param args Arguments to pass to the state and transition handlers.
			 * @return True or false based on whether or not the FSM accepts \p event.
			 * @note This method raises an external event. State handlers should use FsmBase::transition in order to invoke
			 *       a transition.
			 */
			virtual bool raise(FsmEventType &&event, Args &&... args);

			/// @}

		protected:
			QueueType<stl::Pair<FsmEventType, ArgumentWrapper>> _events; //!< Queue of requested transitions.
			mutable LockType _lock; //!< FSM lock.
			mutable Logger _logger; //!< Logging stream.

			static constexpr int Timeout = 200;

			/* METHODS */

			/**
			 * @brief Move \p other into \p this.
			 * @param other Object to move.
			 */
			virtual void move(FsmBase &other);

			/**
			 * @brief Copy an FsmBase object.
			 * @param other FsmBase object to copy.
			 */
			virtual void copy(const FsmBase &other);

			/**
			 * @brief Create an ArgumentWrapper object using move semantics..
			 * @param args Arguments to wrap.
			 * @return ArgumentWrapper based on the value of \p args.
			 * @see stl::Tuple
			 */
			static constexpr auto MakeArgumentWrapper(Args &&... args)
			{
				return stl::MakeTuple(stl::forward<Args>(args)...);
			}

			/// @name Transition function handlers
			/// @{

			/**
			 * @brief Perform a transition.
			 *
			 * @tparam Indices Argument indices.
			 * @param event Alphabet symbol.
			 * @param args Transition arguments.
			 * @return The current FSM state.
			 * @see State
			 * @see Transition
			 * @see FsmEventType
			 * @see _states
			 * @note This method will acquire and release @ref _lock.
			 *
			 * Transition from the current state to the next state based on \p event. The arguments wrapped
			 * by \p args will be passed to the state handlers.
			 */
			template<size_t... Indices>
			FsmStatus transition(FsmEventType &&event, ArgumentWrapper &&args, IndexSequence<Indices...>) noexcept;

			/**
			 * @brief Attempt to transition to the next state.
			 * @return Current FSM state.
			 */
			FsmStatus transition() noexcept;

			///@}

			/// @name Operational methods
			/// @{

			/**
			 * @brief Start the FSM.
			 * @param check Value to indicate whether the FSM should be checked for validity before starting.
			 */
			void start(bool check);

			/**
			 * @brief Stop the FSM.
			 * @param recurse Value to indicate whether the FSM should wait for the FSM to reach a stop state.
			 * @return True or false based on whether the FSM has stopped succesfully or not.
			 */
			bool stop(bool recurse);

			/// @}

		private:
			FsmBase::MapType<FsmStateIdType, stl::SharedPointer<StateType>> _states; //!< FSM state map.
			FsmBase::ArrayListType<stl::ReferenceWrapper<StateType>> _stop_states; //!< Stop state set.
			stl::ReferenceWrapper<StateType> _start_state; //!< Start state reference.
			FsmStateIdType _current; //!< Current state ID.
			stl::ReferenceWrapper<StateType> _error_state; //!< Error state reference.
			FsmStatus _status; //!< FSM status.
			EventType _stop_event; //!< Stop event. Signalled every time a stop state is executed.
			bool _in_transition; //!< Transition indicator. Set when a state invokes a transition. Cleared after execution.
			SetType<FsmEventType> _alphabet; //!< DFA alphabet definition.
			bool _silent; //!< Flag which indicates if message output is enabled or not.

			/* METHODS */

			/**
			 * @brief Add an event to the event queue.
			 * @param event Event to add.
			 * @param args State handler arguments.
			 * @param front Indicator whether or not the event is added to the front of the queue.
			 *
			 * Add a new event to the event queue. FsmBase::transition will add to the front of the queue, while
			 * FsmBase::raise will add to the back of the queue.
			 */
			void addEvent(FsmEventType &&event, ArgumentWrapper &&args, bool front = true);

			/**
			 * @brief Check whether a state is a stop state or not.
			 * @param state State to check.
			 * @return True or false based on whether \p state is a stop state or not.
			 */
			bool isStopState(const FsmStateIdType &state) const;

			/**
			 * @brief Transition to the error state.
			 * @param args Arguments to pass to the state handler.
			 */
			void toErrorState(Args &&... args);

			/**
			 * @brief Update the DFA alphabet based on a state's transition set.
			 * @param state State object to use as the basis for the update.
			 */
			void updateAlphabet(const StateType& state);
		};

		template<typename P, typename W, typename R, typename... Args>
		FsmBase<R(Args...), P, W>::FsmBase(time_t tmo) : Base(tmo), _lock(true), _logger("fsm", stdout),
		                                                 _current(), _status(FsmStatus::Stopped), _in_transition(false),
		                                                 _silent(false)
		{
            this->_logger.setVisibility(Logger::Visibility::Info);
            this->_logger.setStreamVisibility(Logger::Visibility::Info);
		}

		template<typename P, typename W, typename R, typename... Args>
		FsmBase<R(Args...), P, W>::FsmBase(bool silent, time_t tmo) : Base(tmo), _lock(true), _logger("fsm", stdout),
		                                                              _current(), _status(FsmStatus::Stopped), _in_transition(false),
		                                                              _silent(silent)
		{
			this->_logger.setVisibility(Logger::Visibility::Info);
			this->_logger.setStreamVisibility(Logger::Visibility::Info);
		}

		template<typename P, typename W, typename R, typename... Args>
		FsmBase<R(Args...), P, W>::FsmBase(const FsmBase &other)  : Base(), _lock(true),
		                                                            _current(), _status(FsmStatus::Stopped),
		                                                            _in_transition(false), _silent(false)
		{
			FsmBase::copy(other);
		}

		template<typename P, typename W, typename R, typename... Args>
		FsmBase<R(Args...), P, W>::FsmBase(FsmBase &&other) noexcept : Base(), _lock(true), _current(),
		                                                               _status(FsmStatus::Stopped), _in_transition(false),
		                                                               _silent(false)
		{
			FsmBase::move(other);
		}

		template<typename P, typename W, typename R, typename... Args>
		FsmBase<R(Args...), P, W>::~FsmBase()
		{
			UniqueLock<LockType>  lock(this->_lock);

			this->_status = FsmStatus::Stopped;
			this->_events.clear();
			this->_stop_states.clear();
			this->_states.clear();
			this->_stop_event.signal();
			this->_alphabet.clear();
		}

		template<typename P, typename W, typename R, typename... Args>
		FsmBase<R(Args...), P, W> &FsmBase<R(Args...), P, W>::operator=(const FsmBase &other)
		{
			this->copy(other);
			return *this;
		}

		template<typename P, typename W, typename R, typename... Args>
		FsmBase<R(Args...), P, W> &FsmBase<R(Args...), P, W>::operator=(FsmBase &&other) noexcept
		{
			this->move(other);
			return *this;
		}

		template<typename P, typename W, typename R, typename... Args>
		void FsmBase<R(Args...), P, W>::halt()
		{
			UniqueLock<LockType> lock(this->_lock);

			if(!this->running())
				return;

			this->_status = FsmStatus::Stopped;
		}

		template<typename P, typename W, typename R, typename... Args>
		bool FsmBase<R(Args...), P, W>::stop(bool recurse)
		{
			UniqueLock<LockType> lock(this->_lock);

			if(!this->running())
				return true;

			auto &current = this->_states.at(this->_current);

			if(current->id() == this->_error_state->id()) {
				this->_status = FsmStatus::Stopped;
				return true;
			}

			for(auto &state : this->_stop_states) {
				if(state->id() == current->id()) {
					this->_status = FsmStatus::Stopped;
					return true;
				}
			}

			if(!recurse)
				return false;

			this->_stop_event.wait(lock);
			return stop(false);
		}

		template<typename P, typename W, typename R, typename... Args>
		void FsmBase<R(Args...), P, W>::start(bool check)
		{
			UniqueLock<LockType> lock(this->_lock);

			if(check && !this->valid())
				return;

			this->_current = this->_start_state->id();
			this->_status = FsmStatus::Running;
		}

		template<typename P, typename W, typename R, typename... Args>
		inline bool FsmBase<R(Args...), P, W>::running() const
		{
			UniqueLock<LockType> lock(this->_lock);
			return this->_status == FsmStatus::Running;
		}

		template<typename P, typename W, typename R, typename... Args>
		bool FsmBase<R(Args...), P, W>::accept(const FsmEventType &event) const
		{
			UniqueLock<LockType> lock(this->_lock);

			if(!this->running())
				return false;

			auto state = this->current();

			do {
				for(const TransitionType &transition : state->transitions()) {
					auto &tmp = transition.event();

					if(tmp == event)
						return true;
				}

				if(!state->parent())
					break;

				state = this->_states.at(state->parent());
			} while(state);


			return false;
		}

		template<typename P, typename W, typename R, typename... Args>
		bool FsmBase<R(Args...), P, W>::valid() const
		{
			UniqueLock<LockType> lock(this->_lock);

			if(!(this->_status == FsmStatus::Running || this->_status == FsmStatus::Stopped))
				return false;

			if(this->_states.size() == 0)
				return false;

			if(!this->_start_state || this->_stop_states.size() == 0)
				return false;

			if(!this->_error_state)
				return false;

			return this->deterministic();
		}

		template<typename P, typename W, typename R, typename... Args>
		bool FsmBase<R(Args...), P, W>::deterministic() const
		{
			SetType<stl::String> transitions;

			for(const SharedPointer<StateType> &state : this->_states) {
				SetType<FsmEventType> accepts;

				FsmStateIdType id = state->id();
				SharedPointer<StateType> parent;

				do {
					parent = this->_states.at(id);

					for(auto t : parent->transitions()) {
						auto rv = accepts.insert(t.event());

						if(!rv.second) {
							if(!this->_silent)
								this->_logger << "FSM is not deterministic" << Logger::newline;

							return false;
						}
					}

					id = parent->parent();
				} while(parent->hasParent());

				if(this->_alphabet != accepts && state->hasAction()) {
					if(this->_silent)
						this->_logger << "FSM is not deterministic" << Logger::newline;

					return false;
				}
			}

			return true;
		}

		template<typename P, typename W, typename R, typename... Args>
		stl::SharedPointer<State<P, Function<R(Args...)>, R, Args...>> FsmBase<R(Args...), P, W>::current() const
		{
			UniqueLock<LockType> lock(this->_lock);

			if(!this->running())
				return stl::MakeShared<StateType>();

			return this->_states.at(this->_current);
		}

		template<typename P, typename W, typename R, typename... Args>
		FsmStatus FsmBase<R(Args...), P, W>::status() const
		{
			UniqueLock<LockType> lock(this->_lock);
			return this->_status;
		}

		template<typename P, typename W, typename R, typename... Args>
		bool FsmBase<R(Args...), P, W>::addAlphabetSymbol(const FsmEventType &event)
		{
			auto rv = this->_alphabet.insert(event);
			return rv.second;
		}

		template<typename P, typename W, typename R, typename... Args>
		bool FsmBase<R(Args...), P, W>::addAlphabetSymbol(FsmEventType &&event)
		{
			auto rv = this->_alphabet.emplace(stl::forward<FsmEventType>(event));
			return rv.second;
		}

		template<typename P, typename W, typename R, typename... Args>
		stl::Pair<FsmStateId, bool> FsmBase<R(Args...), P, W>::addState(StateType &&state)
		{
			UniqueLock<LockType> lock(this->_lock);
			auto id = state.id();

			if(this->_states.contains(id))
				return stl::Pair<FsmStateIdType, bool>(id, false);

			this->_logger << "Adding state " << state.id() << Logger::newline;

			this->updateAlphabet(state);
			auto ptr = stl::MakeShared<StateType>(stl::forward<StateType>(state));
			auto pair = stl::Pair<FsmStateIdType, bool>(id, false);
			auto rv = this->_states.emplace(stl::move(id), stl::move(ptr));
			pair.second = rv.second;

			return pair;
		}

		template<typename P, typename W, typename R, typename... Args>
		stl::Pair<FsmStateId, bool>
		FsmBase<R(Args...), P, W>::addState(const StateType &state)
		{
			UniqueLock<LockType> lock(this->_lock);
			auto id = state.id();

			if(this->_states.contains(id))
				return stl::Pair<FsmStateIdType, bool>(id, false);

			this->_logger << "Adding state " << state.id() << Logger::newline;

			this->updateAlphabet(state);
			auto ptr = stl::MakeShared<StateType>(state);
			auto pair = stl::Pair<FsmStateIdType, bool>(id, false);
			auto rv = this->_states.emplace(stl::move(id), stl::move(ptr));
			pair.second = rv.second;

			return stl::move(pair);
		}

		template<typename P, typename W, typename R, typename... Args>
		bool FsmBase<R(Args...), P, W>::addStates(ArrayListType<StateType> &states)
		{
			UniqueLock<LockType> lock(this->_lock);

			for(auto &state : states) {
				auto id = state.id();

				if(this->_states.contains(id))
					return false;

				this->updateAlphabet(state);
				auto ptr = stl::MakeShared<StateType>(stl::forward<StateType>(state));

				auto rv = this->_states.emplace(stl::move(id), stl::move(ptr));
				this->_logger << "Adding state " << state.id() << Logger::newline;

				if(!rv.second)
					return false;
			}

			return true;
		}

		template<typename P, typename W, typename R, typename... Args>
		void FsmBase<R(Args...), P, W>::setStartState(const FsmStateIdType &id)
		{
			UniqueLock<LockType> lock(this->_lock);

			if(!this->_states.contains(id))
				return;

			auto ptr = this->_states.at(id);
			this->_start_state = *ptr;
		}

		template<typename P, typename W, typename R, typename... Args>
		bool FsmBase<R(Args...), P, W>::setErrorState(const FsmStateIdType &id)
		{
			UniqueLock<LockType> lock(this->_lock);

			if(!this->_states.contains(id))
				return false;

			this->_error_state = *this->_states.at(id);
			return true;
		}

		template<typename P, typename W, typename R, typename... Args>
		bool FsmBase<R(Args...), P, W>::addStopState(const FsmStateIdType &state)
		{
			UniqueLock<LockType> lock(this->_lock);

			if(!this->_states.contains(state))
				return false;

			auto ptr = this->_states.at(state);
			stl::ReferenceWrapper<StateType> ref(*ptr);
			this->_stop_states.push_back(stl::move(ref));

			return true;
		}

		template<typename P, typename W, typename R, typename... Args>
		bool FsmBase<R(Args...), P, W>::addStopStates(const ArrayListType<FsmStateIdType> &states)
		{
			UniqueLock<LockType> lock(this->_lock);

			/*
			 * Do not merge the loops below. It is critical that either all states are
			 * added succesfully or none at all.
			 */
			for(auto &state: states) {
				if(!this->_states.contains(state))
					return false;
			}

			for(auto &state: states) {
				auto ptr = this->_states.at(state);
				stl::ReferenceWrapper<StateType> ref(*ptr);
				this->_stop_states.push_back(stl::move(ref));
			}

			return true;
		}

		template<typename P, typename W, typename R, typename... Args>
		bool FsmBase<R(Args...), P, W>::raise(FsmEventType &&event, Args &&... args)
		{
			UniqueLock<LockType> lock(this->_lock);

			if(!this->accept(event))
				return false;

			auto wrapper = MakeArgumentWrapper(stl::forward<Args>(args)...);
			this->addEvent(stl::forward<FsmEventType>(event), stl::move(wrapper), false);

			return true;
		}

		template<typename P, typename W, typename R, typename... Args>
		bool FsmBase<R(Args...), P, W>::transition(FsmEventType &&event, Args &&... args)
		{
			UniqueLock<LockType> lock(this->_lock);

			if(!this->accept(event) || this->_in_transition)
				return false;

			auto wrapper = MakeArgumentWrapper(stl::forward<Args>(args)...);
			this->addEvent(stl::forward<FsmEventType>(event), stl::move(wrapper));
			this->_in_transition = true;

			return true;
		}

		template<typename P, typename W, typename R, typename... Args>
		template<size_t... Indices>
		FsmStatus FsmBase<R(Args...), P, W>::transition(FsmEventType &&event, ArgumentWrapper &&args,
		                                                IndexSequence<Indices...>) noexcept
		{
			UniqueLock<LockType> lock(this->_lock);
			stl::ReferenceWrapper<TransitionType> transition;
			stl::SharedPointer<StateType> state;
			bool success;

			this->_watchdog.reset();

			if(!this->running())
				return this->_status;

			/*
			 * Find the transition for the given input symbol. Past this point we
			 * only need to verify that an actual transition was found, verifying
			 * the actual transition itself or the state it points is not necessary
			 * because:
			 *
			 * a) A verified running FSM is deterministic;
			 * b) We verified that the current state accepts the given input
			 *    symbol atleast twice.
			 */
			auto id = this->_current;

			do {
				state = this->_states.at(id);

				for(auto &t : state->transitions()) {
					if(t == event) {
						transition = t;
						break;
					}
				}

				if(transition)
					break;

				id = state->parent();
			} while(unlikely(state->hasParent()));

			if(!transition)
				return FsmStatus::StateUnchanged;

			if(transition->hasGuard()) {
				ArgumentWrapper copy(args);

				if(!transition->guard(stl::get<Indices>(stl::move(copy))...))
					return FsmStatus::StateUnchanged;
			}

			auto nextId = transition->next();
			this->_current = nextId;
			auto next = this->_states.at(nextId);

			ArgumentWrapper copy(args);

			if(next->hasAction())
				success = next->action(stl::get<Indices>(stl::move(args))...);
			else
				success = false;

			if(unlikely(!success)) {
				this->toErrorState(stl::get<Indices>(stl::move(copy))...);
				this->_stop_event.signal();

				this->_logger << "Unable to succesfully execute FSM state!" << Logger::newline;

				return FsmStatus::Fault;
			}

			if(this->isStopState(nextId))
				this->_stop_event.signal();

			return FsmStatus::StateChanged;
		}

		template<typename P, typename W, typename R, typename... Args>
		FsmStatus FsmBase<R(Args...), P, W>::transition() noexcept
		{
			UniqueLock<LockType> lock(this->_lock);

			if(this->_events.size() == 0UL)
				return FsmStatus::StateUnchanged;

			auto value = stl::move(*this->_events.begin());
			this->_events.erase(this->_events.begin());
			auto rv = this->transition(stl::move(value.first), stl::move(value.second),
			                           typename MakeIndexSequence<sizeof...(Args)>::Type());
			this->_in_transition = false;

			return rv;
		}

		template<typename P, typename W, typename R, typename... Args>
		bool FsmBase<R(Args...), P, W>::isStopState(const FsmStateIdType &state) const
		{
			for(const stl::ReferenceWrapper<StateType> &ref : this->_stop_states) {
				if(ref->id() == state)
					return true;
			}

			return false;
		}

		template<typename P, typename W, typename R, typename... Args>
		void FsmBase<R(Args...), P, W>::addEvent(FsmEventType &&event, ArgumentWrapper &&args, bool front)
		{
			stl::Pair<FsmEventType, ArgumentWrapper> value(stl::forward<FsmEventType>(event), stl::move(args));

			if(front)
				this->_events.push_front(stl::move(value));
			else
				this->_events.push_back(stl::move(value));
		}

		template<typename P, typename W, typename R, typename... Args>
		void FsmBase<R(Args...), P, W>::toErrorState(Args &&... args)
		{
			this->_current = this->_error_state->id();
			this->_status = FsmStatus::Error;
			this->_error_state->action(stl::forward<Args>(args)...);
		}

		template<typename P, typename W, typename R, typename... Args>
		void FsmBase<R(Args...), P, W>::copy(const FsmBase &other)
		{
			UniqueLock<LockType> l1(this->_lock);
			UniqueLock<LockType> l2(other._lock);

			this->_events = other._events;
			this->_states = other._states;
			this->_stop_states = other._stop_states;
			this->_start_state = other._start_state;
			this->_current = other._current;
			this->_error_state = other._error_state;
			this->_status = other._status;
			this->_in_transition = other._in_transition;
			this->_logger = other._logger;
			this->_silent = other._silent;
		}

		template<typename P, typename W, typename R, typename... Args>
		void FsmBase<R(Args...), P, W>::move(FsmBase &other)
		{
			using stl::swap;
			swap(*this, other);
		}

		template<typename P, typename W, typename R, typename... Args>
		void FsmBase<R(Args...), P, W>::updateAlphabet(const StateType& state)
		{
			for(auto& transition : state.transitions()) {
				this->_alphabet.insert(transition.event());
			}
		}
	}

/**
 * @brief Event signal type definition.
 *
 * Default FSM event handler argument type.
 */
class Signal {
	public:
		/**
		 * @brief Construct a new Signal type.
		 * @param now Current timestamp.
		 */
		explicit Signal(time_t now = lwiot_tick_ms()) : _moment(now)
		{ }

		/**
		 * @brief Timestamp getter.
		 * @return The timestamp at which the signal was created.
		 */
		time_t time() const
		{
			return this->_moment;
		}

	private:
		time_t _moment;
	};

	/**
	 * @brief Type cast a base signal to a derived class.
	 * @tparam T Requested signal type.
	 * @param value Base signal pointer.
	 * @return Shared pointer of type \p T.
	 */
	template <typename T>
	inline stl::SharedPointer<T> SignalAs(stl::SharedPointer<Signal> value)
	{
		return stl::static_pointer_cast<T>(value);
	}
}
