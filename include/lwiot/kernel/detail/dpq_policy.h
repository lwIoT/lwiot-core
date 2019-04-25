/*
 * Dispatch Queue policy implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot.h>

#include <lwiot/scopedlock.h>
#include <lwiot/stl/linkedlist.h>

#include <lwiot/kernel/lock.h>
#include <lwiot/kernel/event.h>
#include <lwiot/kernel/thread.h>

namespace lwiot
{
	namespace detail
	{
		struct SingleThreading {
			struct Lock {
				constexpr void lock() { }
				constexpr void unlock() { }
			};

			struct Event {
				constexpr void signal() { }
				constexpr void signalFromIrq() { }

				constexpr void wait() noexcept { }
				constexpr bool wait(int tmo) noexcept
				{
					return true;
				}

				constexpr bool wait(ScopedLock& guard, int tmo = FOREVER) noexcept
				{
					return true;
				}
			};

			struct Thread {
				Thread() = default;
				explicit Thread(void *argument = nullptr) { }
				explicit Thread(const char *name, void *argument = nullptr) { }
				explicit Thread(const String& name, void *argument = nullptr) { }
				explicit Thread(const String& name, int priority, size_t stacksize, void *argument = nullptr)
				{ }

				explicit Thread(Thread&& other) = default;
				explicit Thread(Thread&) = delete;
				Thread& operator=(Thread&& rhs) = default;
				Thread& operator=(Thread& rhs) = delete;

				constexpr void start() { }
				constexpr void stop() { }
				constexpr void join() { }
			};
		};

		template <typename T>
		struct SingleThreadPolicy {
			typedef typename stl::LinkedList<T> Queue;
		};

		struct MultiThreading {
			using Event = lwiot::Event;
			using Lock = lwiot::Lock;
			using Thread = lwiot::Thread;
		};

		template <typename T>
		struct MultiThreadingPolicy {
			typedef typename stl::LinkedList<T> Queue;
			using Threading = MultiThreading;
		};

		template <typename T>
		struct HasThreading {
		template <typename U>
			constexpr static traits::TrueType test(typename U::Threading *)
			{
				return traits::TrueType();
			}

			template <typename>
			constexpr static traits::FalseType test(...)
			{
				return traits::FalseType();
			}

			enum {
				value = !! decltype(test<T>(0))()
			};
		};

		template <typename T, bool has_threading>
		struct SelectThreadingPolicy;

		template <typename T>
		struct SelectThreadingPolicy<T, false> {
			using Type = typename T::Threading;
		};

		template <typename T>
		struct SelectThreadingPolicy<T, true> {
			using Type = SingleThreading;
		};
	}
}
