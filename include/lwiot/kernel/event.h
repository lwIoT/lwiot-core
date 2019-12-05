/*
 * Event wrapper class header.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file event.h

#pragma once

#include <lwiot_opts.h>

#include <lwiot/kernel/port.h>
#include <lwiot/kernel/lock.h>
#include <lwiot/kernel/uniquelock.h>

#include <lwiot/scopedlock.h>

namespace lwiot {
	/**
	 * @brief System event.
	 * @ingroup kernel
	 */
	class Event {
	public:
		/**
		 * @brief Construct a new event.
		 * @param length Event queue size.
		 */
		explicit Event(int length = 4);

		explicit Event(const Event& event) = delete;

		/**
		 * @brief Construct a new Event.
		 * @param event Event to move.
		 */
		Event(Event&& event) noexcept ;

		/**
		 * @brief Destroy an event.
		 */
		~Event();

		Event& operator=(const Event& rhs) = delete;

		/**
		 * @brief Destroy an event.
		 * @param rhs Event to move.
		 */
		Event& operator=(Event&& rhs) noexcept ;

		/**
		 * @brief Get the underlying event data.
		 * @return The underlying event data.
		 */
		const lwiot_event_t* getEvent() const;

		/**
		 * @brief Wait for a signal.
		 */
		void wait();

		/**
		 * @brief Wait for a signal.
		 * @param tmo Timeout.
		 * @return Success indicator.
		 * @retval True if an event was received within \p tmo miliseconds.
		 */
		bool wait(int tmo);

		/**
		 * @brief Wait for a signal.
		 * @param tmo Timeout.
		 * @param guard Guard to unlock/lock before and after waiting.
		 * @return Success indicator.
		 * @retval True if an event was received within \p tmo miliseconds.
		 */
		bool wait(ScopedLock& guard, int tmo = FOREVER);

		/**
		 * @brief Wait for a signal.
		 * @tparam T Lock type.
		 * @param tmo Timeout.
		 * @param guard Guard to unlock/lock before and after waiting.
		 * @return Success indicator.
		 * @retval True if an event was received within \p tmo miliseconds.
		 */
		template <typename T>
		bool wait(UniqueLock<T>& guard, int tmo = FOREVER)
		{
			guard.unlock();
			auto rv = this->wait(tmo);
			guard.lock();

			return rv;
		}

		/**
		 * @brief Send a signal to the \p *this from IRQ context.
		 */
		void signalFromIrq();

		/**
		 * @brief Send a signal to the \p *this.
		 */
		void signal();

	private:
		lwiot_event_t* _event;
		int _waiters;
		Lock _lock;
	};
}
