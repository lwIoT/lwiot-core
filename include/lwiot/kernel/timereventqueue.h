/*
 * Timed event queue implementation.
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
#include <lwiot/kernel/eventqueue.h>

#include <lwiot/stl/skiplist.h>
#include <lwiot/stl/vector.h>
#include <lwiot/stl/move.h>
#include <lwiot/stl/string.h>

namespace lwiot
{
	class TimerEventQueue : public EventQueue<String, bool(time_t, time_t)> {
		typedef EventQueue<String, bool(time_t, time_t)> Base;

	public:
		explicit TimerEventQueue();
		explicit TimerEventQueue(time_t feed, time_t freq = 1000);
		TimerEventQueue(TimerEventQueue&& other) noexcept;
		~TimerEventQueue() override;

		TimerEventQueue& operator=(TimerEventQueue&& rhs) noexcept;

		void feed();
		void feed_irq();

		void at(time_t time, const String& event);
		void at(const DateTime& dt, const String& event);
		void at(const DateTime& dt, const String& event, typename Base::HandlerType& handler);

		template <typename Func>
		void at(const DateTime& dt, const String& event, Func&& handler)
		{
			typename Base::HandlerType func(handler);
			this->at(dt, event, handler);
		}

		void once(time_t ms, const String& event);
		void in(time_t time, const String& event);
		void in(time_t time, String&& event);

		void every(time_t interval, const String& event, typename Base::HandlerType& handler);

		template <typename Func>
		void every(time_t interval, const String& event, Func&& func)
		{
			typename Base::HandlerType handler(func);
			this->every(interval, event, handler);
		}

		void remove(const String& event) override;

		void process_ticks();

		time_t offset() const;
		time_t period() const;
		time_t ticks() const;

		friend void swap(TimerEventQueue& teq1, TimerEventQueue& teq2);

		void set(time_t ticks);
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
