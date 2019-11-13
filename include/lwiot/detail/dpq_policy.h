/*
 * Dispatch queue policy implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/kernel/event.h>
#include <lwiot/kernel/lock.h>
#include <lwiot/stl/linkedlist.h>
#include <lwiot/kernel/functionalthread.h>

namespace lwiot
{
	namespace detail
	{
		/* --- Single-threading policies --- */
		struct SingleThreading {
			struct Lock {
				constexpr Lock(bool param = false)
				{ }

				constexpr void lock()
				{
				}

				constexpr void unlock()
				{
				}
			};

			struct Event {
				constexpr void signal()
				{
				}

				constexpr void signalFromIrq()
				{
				}

				constexpr void wait() noexcept
				{
				}

				constexpr bool wait(int tmo) noexcept
				{
					return true;
				}

				constexpr bool wait(UniqueLock<Lock> &guard, int tmo = FOREVER) noexcept
				{
					return true;
				}
			};

			struct Thread {
				Thread() = default;

				explicit Thread(void *argument = nullptr)
				{
				}

				explicit Thread(const char *name, void *argument = nullptr)
				{
				}

				explicit Thread(const String &name, void *argument = nullptr)
				{
				}

				explicit Thread(const String &name, int priority, size_t stacksize, void *argument = nullptr)
				{
				}

				Thread(Thread &&other) = default;

				explicit Thread(Thread &) = delete;

				Thread &operator=(Thread &&rhs) = default;

				Thread &operator=(Thread &rhs) = delete;

				constexpr void start()
				{
				}

				constexpr void stop()
				{
				}

				constexpr void join()
				{
				}
			};
		};

		template <typename T>
		struct SingleThreadingPolicy {
			using Queue = lwiot::stl::LinkedList<T>;
		};

		/* --- Multi-threading policies --- */
		struct MultiThreading {
			using Event  = lwiot::Event;
			using Lock   = lwiot::Lock;
			using Thread = lwiot::FunctionalThread;
		};

		template <typename T>
		struct MultiThreadingPolicy {
			using Queue     = lwiot::stl::LinkedList<T>;
			using Threading = MultiThreading;
		};

		/* --- Check for threading at compile time --- */

		template <typename T>
		struct HasThreading {
		public:
			template <typename U>
			static constexpr traits::TrueType test(typename U::Threading *)
			{
				return {};
			}

			template <typename >
			static constexpr traits::FalseType test(...)
			{
				return {};
			}

		private:
			enum data {
				value = !! decltype(test<T>(0))()
			};

		public:
			static constexpr bool Value = data::value;
		};

		/* --- Policy selection --- */

		template <typename T, bool has_threading>
		struct SelectThreadingPolicy;

		template <typename T>
		struct SelectThreadingPolicy<T, true> {
			typedef typename T::Threading Type;
		};

		template <typename T>
		struct SelectThreadingPolicy<T, false> {
			typedef SingleThreading Type;
		};
	}
}
