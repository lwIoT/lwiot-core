/*
 * Dispatch Queue implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot.h>
#include <stddef.h>

#include <lwiot/kernel/thread.h>
#include <lwiot/kernel/event.h>
#include <lwiot/kernel/atomic.h>

#include "detail/dpq_policy.h"
#include "detail/dpq_base.h"


namespace lwiot
{
	template <typename W>
	using MultiThreadingPolicy = detail::MultiThreadingPolicy<W>;
	template <typename W>
	using SingleThreadingPolicy = detail::SingleThreadPolicy<W>;

#ifdef __GNUG__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif


	namespace detail
	{
		template <typename Proto, template <typename > typename P>
		using DpqBase = detail::DispatchQueueBase<Proto, P>;
	}

	template <typename Proto, template <typename> typename Policy>
	class DispatchQueue :
			public detail::DpqBase<Proto, Policy>,
			private detail::DpqBase<Proto, Policy>::Threading::Thread {
	protected:
		using LockGuard = typename detail::DpqBase<Proto, Policy>::LockGuard;
		typedef detail::DispatchQueueBase<Proto, Policy> BaseClass;

	public:
		DispatchQueue() : BaseClass(), Thread("io"), _running(false)
		{
		}

		DispatchQueue(int cap) : BaseClass(cap), Thread("io"), _running(false)
		{
		}

		virtual ~DispatchQueue()
		{
			this->disable();
		}

		inline void enable() noexcept
		{
			BaseClass::_lock.lock();
			this->_running = true;
			BaseClass::_lock.unlock();

			this->start();
		}

		inline void disable() noexcept
		{
			LockGuard g(BaseClass::_lock);

			if(!this->_running) {
				print_dbg("DPQ is already disabled!\n");
				return;
			}

			this->_running = false;
			g.unlock();

			this->clear_events();
			this->join();
			this->stop();
		}

	protected:
		void run() override
		{
			while(this->isRunning()) {
				auto result = this->process();

				if(result == 0L) {
					this->wait_for();
				} else if(result == -ETRYAGAIN) {
					lwiot_sleep(1);
				}
			}

			print_dbg("Dispatch queue stopping!\n");
		}

		bool isRunning() noexcept
		{
			BaseClass::_lock.lock();
			auto value = this->_running;
			BaseClass::_lock.unlock();

			return value;
		}

	private:
		bool _running;
	};

#ifdef __GNUG__
#pragma GCC diagnostic pop
#endif

	template <typename Proto>
	class DispatchQueue<Proto, SingleThreadingPolicy> : public detail::DispatchQueueBase<Proto, SingleThreadingPolicy > {
		typedef detail::DispatchQueueBase<Proto, SingleThreadingPolicy > BaseClass;
	public:

		constexpr DispatchQueue() : BaseClass()
		{ }

		constexpr DispatchQueue(int capacity) : BaseClass(capacity)
		{ }

		constexpr void enable() const
		{ }

		constexpr void disable() const
		{ }
	};
}
