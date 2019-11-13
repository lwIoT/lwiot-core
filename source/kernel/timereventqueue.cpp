/*
 * Timed event queue implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <lwiot.h>
#include <stddef.h>

#include <lwiot/kernel/thread.h>
#include <lwiot/kernel/event.h>
#include <lwiot/kernel/atomic.h>
#include <lwiot/kernel/timereventqueue.h>

#include <lwiot/stl/skiplist.h>
#include <lwiot/stl/move.h>
#include <lwiot/stl/string.h>

#include <lwiot/util/datetime.h>

namespace lwiot
{
	TimerEventQueue::TimerEventQueue() : Base(), _tick(0UL), _work(), _lock(), _offset(0), _period(1000/RATE)
	{
		this->_offset = lwiot_tick_ms() / this->_period;
		this->enable();
	}

	TimerEventQueue::TimerEventQueue(time_t feed, time_t freq) : Base(),
		_tick(feed), _work(), _lock(), _offset(0), _period(1000/freq)
	{
		this->_offset = lwiot_tick_ms() / this->_period;
		this->enable();
	}

	TimerEventQueue::TimerEventQueue(lwiot::TimerEventQueue &&other) noexcept :
		Base(stl::forward<EventQueue>(other)), _tick(stl::move(other._tick)), _work(stl::move(other._work)),
		_lock(), _offset(stl::move(other._offset)), _period(stl::move(other._offset))
	{
	}

	TimerEventQueue::~TimerEventQueue()
	{
		Base::LockGuard g(this->_lock);
		this->_work.clear();
	}

	TimerEventQueue& TimerEventQueue::operator=(lwiot::TimerEventQueue &&rhs) noexcept
	{
		Base::LockGuard g(this->_lock);

		using stl::swap;
		swap(*this, rhs);

		Base::operator=(stl::forward<Base>(rhs));
		return *this;
	}

	void swap(TimerEventQueue& evq1, TimerEventQueue& evq2)
	{
		evq1._work = stl::move(evq2._work);
		evq1._tick = stl::move(evq2._tick);
		evq2._offset = stl::move(evq2._offset);
	}

	void TimerEventQueue::at(const lwiot::DateTime &dt, const lwiot::String &event)
	{
		TimerWork w{};
		Base::LockGuard g(this->_lock);

		w.repeating = false;
		w.event = event;
		w.moment = dt;
		w.moment *= 1000;
		w.moment = this->ms_to_ticks(w.moment);
		w.moment -= this->_offset;
		this->_work.push_back(stl::move(w));
	}

	void TimerEventQueue::at(time_t time, const lwiot::String &event)
	{
		TimerWork w{};
		Base::LockGuard g(this->_lock);

		w.repeating = false;
		w.event = event;
		w.moment = this->ms_to_ticks(time);
		this->_work.push_back(stl::move(w));
	}


	void TimerEventQueue::at(const lwiot::DateTime &dt, const lwiot::String &event, typename Base::HandlerType &handler)
	{
		TimerWork w{};

		this->on(event, handler);

		w.moment = dt;
		w.moment *= 1000;
		w.moment = this->ms_to_ticks(w.moment);
		w.moment -= this->_offset;
		w.event = event;

		Base::LockGuard g(this->_lock);
		this->_work.push_back(stl::move(w));
	}

	void TimerEventQueue::every(time_t interval, const lwiot::String &event, Base::HandlerType &handler)
	{
		TimerWork w{};

		w.event = event;
		w.interval = this->ms_to_ticks(interval);
		w.repeating = true;
		w.moment = this->_tick.load() + w.interval;

		this->on(event, handler);

		Base::LockGuard g(this->_lock);
		this->_work.push_back(stl::move(w));
	}

	void TimerEventQueue::once(time_t ms, const lwiot::String &event)
	{
		this->in(ms, event);
	}

	void TimerEventQueue::in(time_t time, const lwiot::String &event)
	{
		TimerWork w{};
		Base::LockGuard g(this->_lock);

		w.repeating = false;
		w.event = event;
		w.moment = this->_tick.load() + this->ms_to_ticks(time);
		this->_work.push_back(stl::move(w));
	}

	void TimerEventQueue::in(time_t time, lwiot::String &&event)
	{
		TimerWork w{};

		w.repeating = false;
		w.moment = this->_tick.load() + this->ms_to_ticks(time);
		w.event = stl::forward<String>(event);

		Base::LockGuard g(this->_lock);
		this->_work.push_back(stl::move(w));
	}

	void TimerEventQueue::process_ticks()
	{
		time_t now ;
		Base::LockGuard g(this->_lock);

		for(auto iter = this->_work.begin(); iter != this->_work.end(); ++iter) {
			auto& work = *iter;
			now = this->_tick.load();

			if(now >= work.moment) {
				if(work.repeating)
					work.moment = now + work.interval;

				this->signal(work.event, stl::move(now));

				if(!work.repeating)
					this->_work.erase(iter);
			}
		}

	}

	void TimerEventQueue::feed()
	{
		this->_tick += 1;
	}

	void TimerEventQueue::feed_irq()
	{
		this->_tick += 1;
	}

	time_t TimerEventQueue::offset() const
	{
		return this->_offset;
	}

	void TimerEventQueue::remove(const lwiot::String &event)
	{
		Base::LockGuard g(this->_lock);

		stl::foreach(this->_work, [&](auto& iter) {
			if(iter->event == event) {
				this->_work.erase(iter);
			}
		});

		g.unlock();
		Base::remove(event);
	}

	void TimerEventQueue::set(time_t ticks)
	{
		this->_tick.store(ticks);
	}

	void TimerEventQueue::set(const lwiot::DateTime &time)
	{
		time_t ms = time;

		ms *= 1000;
		auto ticks = this->ms_to_ticks(ms);
		this->_tick.store(ticks);
	}

	time_t TimerEventQueue::period() const
	{
		return this->_period;
	}

	time_t TimerEventQueue::ticks() const
	{
		return this->_tick.load();
	}
}
