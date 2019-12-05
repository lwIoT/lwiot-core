/*
 * Event queue implementation based on the dispatch queue.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file eventqueue.h

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
#ifdef DOXYGEN
	/**
	 * @brief Event queue.
	 * @tparam Event Event type.
	 * @tparam Proto Handler prototype.
	 * @ingroup kernel
	 */
	template <typename Event, typename Proto>
	class EventQueue
#else
	template <typename Event, typename Proto>
	class EventQueue;

	template <typename E, typename R, typename Time, typename ... Args>
	class EventQueue<E, R(Time, Args...)> : public DispatchQueue<R(Time, Args...), MultiThreadingPolicy>
#endif
	{
		typedef DispatchQueue<R(Time, Args...), MultiThreadingPolicy> Base;

	public:
		typedef E event_type; //!< Event type.

		/**
		 * @brief Construct a new event queue.
		 */
		explicit EventQueue() : Base(), _handlers()
		{
		}

		EventQueue(const EventQueue &) = delete;

		/**
		 * @brief Construct a new event queue.
		 * @param other Event to move.
		 */
		EventQueue(EventQueue &&other) noexcept : Base(stl::forward<Base>(other)), _handlers(stl::move(other._handlers)),
		                                          _lock()
		{
		}

		/**
		 * @brief Event to destroy.
		 */
		~EventQueue() override
		{
			typename Base::LockGuard g(this->_lock);
			this->_handlers.clear();
		}

		EventQueue &operator=(const EventQueue &) = delete;

		/**
		 * @brief Move assign an event queue..
		 * @param rhs Event to move.
		 * @return A reference to \p *this.
		 */
		EventQueue &operator=(EventQueue &&rhs) noexcept
		{
			typename Base::LockGuard g(this->_lock);
			Base::operator=(stl::forward<Base>(rhs));
			using stl::swap;

			swap(this->_handlers, rhs._handlers);
			return *this;
		}

		/**
		 * @brief Add a handler.
		 * @tparam Func Handler prototype.
		 * @param name Event.
		 * @param handler Handler.
		 */
		template <typename Func>
		void on(event_type&& name, Func&& handler)
		{
			typename Base::LockGuard g(this->_lock);
			this->_handlers.insert(stl::move(name), stl::forward<Func>(handler));
		}

		/**
		 * @brief Add a handler.
		 * @param name Event.
		 * @param handler Handler.
		 */
		void on(const event_type& name, const typename Base::HandlerType& handler)
		{
			typename Base::LockGuard g(this->_lock);
			this->_handlers.insert(name, handler);
		}

		/**
		 * @brief Signal an event.
		 * @tparam T Dummy template parameter.
		 * @param name Event name.
		 * @param args Event handler arguments.
		 */
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

		/**
		 * @brief Signal an event.
		 * @tparam T Dummy template parameter.
		 * @param name Event name.
		 * @param args Event handler arguments.
		 */
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

		/**
		 * @brief Remove an event.
		 * @param name Event to remove.
		 */
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
