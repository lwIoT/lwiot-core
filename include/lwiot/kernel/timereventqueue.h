/*
 * Timed event queue implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file timereventqueue.h

#pragma once

#include <lwiot.h>
#include <stddef.h>

#include <lwiot/kernel/thread.h>
#include <lwiot/kernel/event.h>
#include <lwiot/kernel/atomic.h>
#include <lwiot/kernel/dispatchqueue.h>
#include <lwiot/kernel/eventqueue.h>

#include <lwiot/stl/skiplist.h>
#include <lwiot/stl/vector.h>
#include <lwiot/stl/move.h>
#include <lwiot/stl/string.h>

namespace lwiot
{
	/**
	 * @brief Event queue based on an (external) timer source.
	 * @ingroup kernel
	 */
	class TimerEventQueue : public EventQueue<String, bool(time_t, time_t)> {
		typedef EventQueue<String, bool(time_t, time_t)> Base;

	public:
		/**
		 * @brief Construct a new timer event queue object.
		 */
		explicit TimerEventQueue();

		/**
		 * @brief Construct a new timer event queue object.
		 * @param feed Initial tick count.
		 * @param freq Timer frequency.
		 */
		explicit TimerEventQueue(time_t feed, time_t freq = 1000);

		/**
		 * @brief Move construct a timer event queue.
		 * @param other Queue to move.
		 */
		TimerEventQueue(TimerEventQueue&& other) noexcept;

		/**
		 * @brief Destroy a timer event queue.
		 */
		~TimerEventQueue() override;

		/**
		 * @brief Move assignment operator.
		 * @param rhs Queue to move.
		 */
		TimerEventQueue& operator=(TimerEventQueue&& rhs) noexcept;

		/**
		 * @brief Feed a single tick.
		 */
		void feed();

		/**
		 * @brief Feed a single tick from IRQ context.
		 */
		void feed_irq();

		/**
		 * @brief Trigger an event at a given timestamp.
		 * @param time Point in time.
		 * @param event Event to trigger.
		 */
		void at(time_t time, const String& event);

		/**
		 * @brief Trigger an event at a given timestamp.
		 * @param dt Point in time.
		 * @param event Event to trigger.
		 */
		void at(const DateTime& dt, const String& event);

		/**
		 * @brief Trigger an event at a given timestamp.
		 * @param dt Point in time.
		 * @param event Event to trigger.
		 * @param handler Event handler.
		 */
		void at(const DateTime& dt, const String& event, typename Base::HandlerType& handler);

		/**
		 * @brief Trigger an event at a given timestamp.
		 * @tparam Func Functor type.
		 * @param dt Point in time.
		 * @param event Event to trigger.
		 * @param handler Event handler.
		 */
		template <typename Func>
		void at(const DateTime& dt, const String& event, Func&& handler)
		{
			typename Base::HandlerType func(handler);
			this->at(dt, event, handler);
		}

		/**
		 * @brief Trigger an event once in a given number of miliseconds.
		 * @param ms Timeout.
		 * @param event Event to trigger after \p ms miliseconds.
		 */
		void once(time_t ms, const String& event);

		/**
		 * @brief Trigger \p event in \p time miliseconds.
		 * @param time Milisecond timer value.
		 * @param event Event to trigger.
		 */
		void in(time_t time, const String& event);

		/**
		 * @brief Trigger \p event in \p time miliseconds.
		 * @param time Milisecond timer value.
		 * @param event Event to trigger.
		 */
		void in(time_t time, String&& event);

		/**
		 * @brief Trigger an event at a given interval.
		 * @param interval Event interval.
		 * @param event Event to trigger.
		 * @param handler Event handler.
		 */
		void every(time_t interval, const String& event, typename Base::HandlerType& handler);

		/**
		 * @brief Trigger an event at a given interval.
		 * @tparam Func Functor type.
		 * @param interval Event interval.
		 * @param event Event to trigger.
		 * @param func Event handler.
		 */
		template <typename Func>
		void every(time_t interval, const String& event, Func&& func)
		{
			typename Base::HandlerType handler(func);
			this->every(interval, event, handler);
		}

		/**
		 * @brief Remove a timer event.
		 * @param event Event to remove.
		 */
		void remove(const String& event) override;

		/**
		 * @brief Process timer ticks.
		 */
		void process_ticks();

		/**
		 * @brief Get the offset of this event queue.
		 * @return The current offset.
		 */
		time_t offset() const;

		/**
		 * @brief Get the timer period.
		 * @return The timer period.
		 */
		time_t period() const;

		/**
		 * @brief Get the number of ticks.
		 * @return Number of registered ticks.
		 */
		time_t ticks() const;

		/**
		 * @brief Swap two timer queue objects.
		 * @param teq1 Queue 1.
		 * @param teq2 Queue 2.
		 */
		friend void swap(TimerEventQueue& teq1, TimerEventQueue& teq2);

		/**
		 * @brief Set the current number of ticks.
		 * @param ticks Set the number of ticks.
		 */
		void set(time_t ticks);

		/**
		 * @brief Set the current number of ticks based on a timestmap.
		 * @param time Number of ticks to set.
		 */
		void set(const DateTime& time);

	private:
		struct TimerWork;

		Atomic<time_t> _tick;
		stl::LinkedList<TimerWork> _work;
		Base::Threading::Lock _lock;
		time_t _offset;
		time_t _period;

		static constexpr time_t RATE = 1000;

		inline time_t ms_to_ticks(time_t ms)
		{
			return ms / this->_period;
		}

		struct TimerWork {
			time_t moment;
			stl::String event;

			time_t interval;
			bool repeating;
		};
	};
}
