/*
 * Generic atomic implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot.h>

namespace lwiot
{
	template <typename T>
	class Atomic {
	public:
		explicit Atomic() = default;
		explicit Atomic(const T& value) : _value(value)
		{ }

		void add(const T& value)
		{
			enter_critical();
			this->_value += value;
			exit_critical();
		}

		void substract(const T& value)
		{
			enter_critical();
			this->_value -= value;
			exit_critical();
		}

		T value() const
		{
			enter_critical();
			auto v = this->_value;
			exit_critical();

			return v;
		}

		bool operator==(const Atomic<T>& other)
		{
			enter_critical();
			auto ours = this->_value;
			exit_critical();

			return ours == other.value();
		}

		bool operator!=(const Atomic<T>& other)
		{
			enter_critical();
			auto ours = this->_value;
			exit_critical();

			return ours != other.value();
		}

		explicit operator T() const
		{
			return this->_value;
		}

		T operator++()
		{
			enter_critical();
			this->_value++;
			auto v = this->_value;
			exit_critical();

			return v;
		}

		T operator--()
		{
			enter_critical();
			this->_value--;
			auto v = this->_value;
			exit_critical();

			return v;
		}

		void set(const T& value)
		{
			enter_critical();
			this->_value = value;
			exit_critical();
		}

	private:
		T _value;
	};
}
