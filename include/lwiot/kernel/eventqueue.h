/*
 * Event queue implementation based on the dispatch queue.
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
#include <lwiot/kernel/dispatchqueue.h>

#include <lwiot/stl/skiplist.h>
#include <lwiot/stl/move.h>
#include <lwiot/stl/string.h>

#include <lwiot/util/datetime.h>

namespace lwiot
{
	template <typename Event, typename Proto>
	class EventQueue;

	template <typename E, typename R, typename Time, typename ... Args>
	class EventQueue<E, R(Time, Args...)> : public DispatchQueue<R(Time, Args...), MultiThreadingPolicy> {
		typedef DispatchQueue<R(Time, Args...), MultiThreadingPolicy> Base;

	public:
		typedef E event_type;

		explicit EventQueue() : Base(), _handlers()
		{
		}

		EventQueue(const EventQueue &) = delete;

		EventQueue(EventQueue &&other) noexcept : Base(stl::forward<Base>(other)), _handlers(stl::move(other._handlers)),
		                                          _lock()
		{
		}

		~EventQueue() override
		{
			typename Base::LockGuard g(this->_lock);
			this->_handlers.clear();
		}

		EventQueue &operator=(const EventQueue &) = delete;

		EventQueue &operator=(EventQueue &&rhs) noexcept
		{
			typename Base::LockGuard g(this->_lock);
			Base::operator=(stl::forward<Base>(rhs));
			using stl::swap;

			swap(this->_handlers, rhs._handlers);
			return *this;
		}

		template <typename Func>
		void on(event_type&& name, Func&& handler)
		{
			typename Base::LockGuard g(this->_lock);
			this->_handlers.insert(stl::move(name), stl::forward<Func>(handler));
		}

		void on(const event_type& name, const typename Base::HandlerType& handler)
		{
			typename Base::LockGuard g(this->_lock);
			this->_handlers.insert(name, handler);
		}

		template <typename T = Time, traits::EnableIf_t<traits::IsSame<T, DateTime>::value, int> = 0>
		void signal(const event_type& name, Args&&... args)
		{
			DateTime dt;
			typename Base::LockGuard g(this->_lock);

			auto handler = this->_handlers.at(name);
			if(!handler)
				return;

			this->enqueue(handler, stl::move(dt), stl::forward<Args>(args)...);
		}

		template <typename T = Time, traits::EnableIf_t<traits::IsSame<T, time_t>::value, int> = 0>
		void signal(const event_type & name, Args&&... args)
		{
			auto time = lwiot_tick_ms();
			typename Base::LockGuard g(this->_lock);

			auto& handler = this->_handlers.at(name);

			if(!handler)
				return;

			this->enqueue(handler, stl::move(time), stl::forward<Args>(args)...);
		}

		virtual void remove(const event_type & name)
		{
			typename Base::LockGuard g(this->_lock);
			this->_handlers.erase(name);
		}

	private:
		stl::SkipList<event_type, typename Base::HandlerType> _handlers;
		typename Base::Threading::Lock _lock;
	};
}
