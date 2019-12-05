/*
 * Functional timer implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file functionaltimer.h

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/lwiot.h>
#include <lwiot/log.h>
#include <lwiot/function.h>

#include <lwiot/kernel/timer.h>

#include <lwiot/traits/enableif.h>
#include <lwiot/traits/issame.h>

#include <lwiot/stl/tuple.h>
#include <lwiot/stl/array.h>

#include <tuple>

namespace lwiot
{
	namespace timer_detail
	{
		template <typename Arg>
		struct FunctionalTimer_handler {
		public:
			FunctionalTimer_handler(Arg argument) : _value(argument)
			{ }

			void invoke()
			{
				this->_handler(this->_value);
			}

			template <typename Func>
			constexpr void setHandler(Func&& handler)
			{
				this->_handler = stl::forward<Func>(handler);
			}

		private:
			typedef Function<void(Arg)> HandlerType;

			Arg _value;
			HandlerType _handler;
		};

		template <>
		struct FunctionalTimer_handler<void> {
		public:
			void invoke()
			{
				this->_handler();
			}

			template <typename Func>
			constexpr void setHandler(Func&& handler)
			{
				this->_handler = stl::forward<Func>(handler);
			}

		private:
			typedef Function<void()> HandlerType;
			HandlerType _handler;
		};

	}

	/**
	 * @brief Timer type.
	 * @ingroup kernel
	 */
	enum TimerType {
		Continuous = 0,
		OneShot = 0x1,
	};

	/**
	 * @brief Functional timer object.
	 * @tparam T Parameter type.
	 * @ingroup kernel
	 */
	template <typename T>
	class FunctionalTimer : public Timer {
	private:
		template <typename Type>
		static constexpr bool IsVoid = traits::IsSame<Type, void>::value;
		using ParameterType = T;

	public:
		/**
		 * @brief Construct a new timer object.
		 * @tparam U Dummy parameter.
		 * @param type Timer type.
		 * @param interval Interval.
		 * @param param Parameter value.
		 */
		template <typename U = FunctionalTimer<T>, typename traits::EnableIf<!IsVoid<typename U::ParameterType>, int>::type = 0>
		FunctionalTimer(TimerType type, time_t interval, typename U::ParameterType param) :
			Timer(randstr("ft-", 4), interval, type, nullptr), _handler(param)
		{ }

		/**
		 * @brief Construct a new timer object.
		 * @tparam U Dummy parameter.
		 * @param type Timer type.
		 * @param interval Interval.
		 */
		template <typename U = T, typename traits::EnableIf<traits::IsSame<U, void>::value, int>::type = 0>
		FunctionalTimer(TimerType type, time_t interval) : Timer(randstr("ft-", 4), interval, type, nullptr)
		{ }

		/**
		 * @brief Destory a functional timer object.
		 */
		~FunctionalTimer() override = default;

		/**
		 * @brief Start a functional timer.
		 * @tparam Func Functor type.
		 * @param handler Funcotr object.
		 */
		template <typename Func>
		void start(Func&& handler)
		{
			this->_handler.setHandler(stl::forward<Func>(handler));
			Timer::start();
		}

		using Timer::stop;
		using Timer::reset;

	protected:
		/**
		 * @brief Timer tick handler.
		 */
		void tick() override
		{
			this->_handler.invoke();
		}

	private:
		typename timer_detail::FunctionalTimer_handler<T> _handler;
	};
}
