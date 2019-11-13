/*
 * Dispatch queue implementation.
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

#include <lwiot/detail/dpq_policy.h>
#include <lwiot/detail/dpq_base.h>

namespace lwiot
{
	template <typename W>
	using MultiThreadingPolicy = detail::MultiThreadingPolicy<W>;
	template <typename W>
	using SingleThreadingPolicy = detail::SingleThreadingPolicy<W>;

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
	class DispatchQueue : public detail::DpqBase<Proto, Policy> {
		typedef detail::DpqBase<Proto, Policy> Base;
	public:
		explicit DispatchQueue(int cap, int tmo, const stl::String& name) :
				detail::DpqBase<Proto, Policy>(cap, tmo), _thread(name), _running(false)
		{ }

		explicit DispatchQueue(int cap, int tmo) :
				detail::DpqBase<Proto, Policy>(cap, tmo), _thread("io"), _running(false)
		{ }

		explicit DispatchQueue(const stl::String& name) :
				detail::DpqBase<Proto, Policy>(Base::DefaultCapacity, Base::DefaultTimeout), _thread(name),
				_running(false)
		{ }

		explicit DispatchQueue(int cap, const stl::String& name = "io") :
				detail::DpqBase<Proto, Policy>(cap, Base::DefaultTimeout), _thread(name), _running(false)
		{ }

		DispatchQueue() : detail::DpqBase<Proto, Policy>(), _thread("io"), _running(false)
		{ }

		DispatchQueue(const DispatchQueue&) = delete;
		DispatchQueue& operator=(const DispatchQueue&) = delete;

		DispatchQueue(DispatchQueue&& other) noexcept :
			detail::DpqBase<Proto, Policy>(stl::forward<detail::DpqBase<Proto, Policy>>(other)), _thread("io"),
			_running(other._running)
		{
		}

		~DispatchQueue() override
		{
			this->disable();
		}

		DispatchQueue& operator=(DispatchQueue&& rhs) noexcept
		{
			using stl::swap;
			this->move(rhs);

			return *this;
		}

		inline void enable()
		{
			Base::_lock.lock();
			this->_running = true;
			Base::_lock.unlock();

			this->_thread.start([&]() {
				this->run();
			});
		}

		inline void disable() noexcept
		{
			typename Base::LockGuard g(Base::_lock);

			if(!this->_running) {
				print_dbg("DPQ is already disabled!\n");
				return;
			}

			this->_running = false;
			g.unlock();

			this->clear_waiters();
			this->_thread.join();
		}

		bool running() noexcept
		{
			Base::_lock.lock();
			auto value = this->_running;
			Base::_lock.unlock();

			return value;
		}

	protected:
		void run() noexcept
		{
			while(this->running()) {
				this->_watchdog->reset();
				auto result = this->process();

				if(result == -EOK) {
					this->wait_for();
				}
			}

			print_dbg("Dispatch queue stopping!\n");
		}

	private:
		typename detail::DpqBase<Proto, Policy>::Threading::Thread _thread;
		bool _running;
	};

#ifdef __GNUG__
#pragma GCC diagnostic pop
#endif

	template <typename Proto>
	class DispatchQueue<Proto, SingleThreadingPolicy> : public detail::DispatchQueueBase<Proto, SingleThreadingPolicy > {
		typedef detail::DispatchQueueBase<Proto, SingleThreadingPolicy> BaseClass;
	public:
		explicit DispatchQueue() = default;

		constexpr void enable()
		{ }

		constexpr void disable()
		{ }
	};
}
