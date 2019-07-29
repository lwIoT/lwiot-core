/*
 * EvenQueue definition & implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <lwiot.h>
#include <stddef.h>

#include <lwiot/kernel/thread.h>
#include <lwiot/kernel/event.h>
#include <lwiot/kernel/atomic.h>
#include <lwiot/kernel/dispatchqueue.h>
#include <lwiot/kernel/lock.h>

#include <lwiot/stl/string.h>
#include <lwiot/stl/map.h>

namespace lwiot
{
	template <typename Proto>
	class EventQueue;

	template <typename R, typename Time, typename ... Args>
	class EventQueue<R(Time, Args...)> : public DispatchQueue<R(Time, Args...), MultiThreadingPolicy> {
		static_assert(traits::IsSame<Time, time_t>::value, "First argument of prototype must be of time_t!");
		typedef DispatchQueue<R(Time, Args...), MultiThreadingPolicy> Base;

	public:
		EventQueue() : Base()
		{ }

		EventQueue(int capacity) : Base(capacity)
		{ }

		virtual ~EventQueue()
		{
			if(this->isRunning())
				this->disable();

			typename Base::LockGuard g(this->_lock);
			this->_handlers.~Map();
		}

		template <typename Func>
		void on(stl::String&& key, Func&& handler)
		{
			typename Base::LockGuard g(this->_lock);
			this->_handlers.add(stl::forward<stl::String>(key), stl::forward<Func>(handler));
		}

		template <typename Func>
		void on(const char *str, Func&& handler)
		{
			stl::String s(str);
			this->on(stl::move(s), stl::forward<Func>(handler));
		}

		void signal(const stl::String& key, Args&&... args)
		{
			time_t tick = lwiot_tick_ms();
			typename Base::LockGuard g(this->_lock);

			typename Base::Handler handle = this->_handlers.at(key);
			assert(handle);

			g.unlock();

			this->enqueue(handle, stl::move(tick), stl::forward<Args>(args)...);
		}

		inline void remove(const stl::String& key)
		{
			typename Base::LockGuard g1(this->_lock);
			typename Base::LockGuard g2(Base::BaseClass::_lock);

			this->_handlers.remove(key);
		}

	private:
		stl::Map<stl::String, typename Base::Handler> _handlers;
		Lock _lock;
	};
}
