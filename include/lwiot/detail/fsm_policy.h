/*
 * Finite State Machine policy implementation.
 *
 * @author Michel Megens
 * @email  michel@michelmegens.net
 */

/**
 * @file fsm_policy.h FSM policy definitions.
 */

#pragma once

#include <lwiot.h>

#ifdef HAVE_OS
#include <lwiot/kernel/event.h>
#include <lwiot/kernel/functionalthread.h>
#endif

#include <lwiot/kernel/uniquelock.h>

#include <lwiot/stl/set.h>
#include <lwiot/stl/hashmap.h>
#include <lwiot/stl/linkedlist.h>
#include <lwiot/stl/vector.h>

namespace lwiot
{
	namespace detail_fsm
	{
		/**
		 * @brief Default watchdog definition.
		 * @note This is a dummy watchdog.
		 */
		struct Watchdog {
			constexpr void enable(int timeout) { }
			constexpr void disable() { }
			constexpr void reset() { }
		};

		/**
		 * @brief Threading model for systems that do not have a scheduler.
		 */
		struct SingleThreading {
			/**
			 * @brief Dummy lock.
			 */
			struct Lock {
				constexpr explicit Lock(bool param = false)
				{ }

				constexpr void lock() { }
				constexpr void unlock() { }
			};

			/**
			 * @brief Dummy event.
			 */
			struct Event {
				constexpr void signal() { }
				constexpr void signalFromIrq() { }
				constexpr void wait() noexcept { }
				constexpr void wait(int tmo) noexcept { }
				template <typename T>
				constexpr void wait(UniqueLock<T>& lock) { }
			};

			/**
			 * @brief Dummy thread.
			 */
			struct Thread {
				typedef Function<void(void)> Runner;

				constexpr Thread() = default;
				constexpr explicit Thread(const String& name) { }
				constexpr explicit Thread(const char* name) { }
				constexpr Thread(const String& name, Runner& runner) { }
				~Thread() = default;

				constexpr void start(const Runner& runner) { }
				constexpr void start() { }
				constexpr void stop() { }
				constexpr void join() { }

				template <typename Func>
				constexpr void start(Func&& functor) { }
			};
		};

		/**
		 * @brief FSM policy for systems that do not have scheduler.
		 */
		struct SingleThreadingPolicy {
			using FsmEvent  = uint16_t; //!< FSM event ID type.

			/**
			 * @brief FSM map type.
			 * @tparam K Key type.
			 * @tparam V Value type.
			 */
			template <typename K, typename V>
			using Map = lwiot::stl::HashMap<K,V>;

			/**
			 * @brief Queue type.
			 * @tparam T Value type.
			 */
			template <typename T>
			using Queue = lwiot::stl::LinkedList<T>;

			/**
			 * @brief Dynamic array type.
			 * @tparam T Value type.
			 */
			template <typename T>
			using ArrayList = lwiot::stl::Vector<T>;

			/**
			 * @brief Set type.
			 * @tparam T Value type.
			 */
			template <typename T>
			using Set = lwiot::stl::Set<T>;
		};

#ifdef HAVE_OS
		/**
		 * @brief Threading model for the lwIoT abstraction layer.
		 */
		struct MultiThreading {
			using Thread = lwiot::FunctionalThread; //!< Thread type definition.
			using Event  = lwiot::Event; //!< Event type definition.
			using Lock   = lwiot::Lock; //!< Mutex type definition.
		};

		/**
		 * @brief Multithreading policy for the lwIoT abstraction layer.
		 */
		struct MultiThreadingPolicy {
			using FsmEvent   = uint16_t; //!< FSM event ID type.
			using Threading = MultiThreading; //!< Threading model type definition.

			/**
			 * @brief FSM map type.
			 * @tparam K Key type.
			 * @tparam V Value type.
			 */
			template <typename K, typename V>
			using Map = lwiot::stl::HashMap<K,V>;

			/**
			 * @brief Queue type.
			 * @tparam T Value type.
			 */
			template <typename T>
			using Queue = lwiot::stl::LinkedList<T>;

			/**
			 * @brief Dynamic array type.
			 * @tparam T Value type.
			 */
			template <typename T>
			using ArrayList = lwiot::stl::Vector<T>;

			/**
			 * @brief Set type.
			 * @tparam T Value type.
			 */
			template <typename T>
			using Set = lwiot::stl::Set<T>;
		};
#endif
	}
}
