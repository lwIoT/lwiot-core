/*
 * Generic atomic implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot.h>
#include <lwiot/stl/move.h>

namespace lwiot
{
	namespace detail
	{
#ifndef DOXYGEN
		typedef enum memory_order {
			memory_order_relaxed,
			memory_order_consume,
			memory_order_acquire,
			memory_order_release,
			memory_order_acq_rel,
			memory_order_seq_cst
		} memory_order;
#endif

		template<typename T>
		class Atomic {
		public:
			explicit Atomic() = default;

			explicit Atomic(const Atomic &) = delete;

			explicit Atomic(const T &value) : _value(value)
			{
			}

			Atomic(Atomic&& value) noexcept : _value(value.load())
			{ }

			Atomic &operator=(const Atomic &) = delete;

			constexpr Atomic& operator=(Atomic&& value) noexcept
			{
				enter_critical();
				this->_value = stl::move(value._value);
				exit_critical();

				return *this;
			}

			T fetch_add(const T &value)
			{
				enter_critical();
				auto v = this->_value;
				this->_value += value;
				exit_critical();
				return v;
			}

			T fetch_sub(const T &value)
			{

				enter_critical();
				auto v = this->_value;
				this->_value -= value;
				exit_critical();
				return v;
			}

			T fetch_and(const T &value)
			{
				enter_critical();
				auto v = this->_value;
				this->_value &= value;
				exit_critical();

				return v;
			}

			T fetch_xor(const T &value)
			{
				enter_critical();
				auto v = this->_value;
				this->_value ^= value;
				exit_critical();

				return v;
			}

			T fetch_or(const T &value)
			{
				enter_critical();
				auto v = this->_value;
				this->_value |= value;
				exit_critical();

				return v;
			}

			T load() const
			{
				enter_critical();
				auto v = this->_value;
				exit_critical();

				return v;
			}

			void store(T v, memory_order order = memory_order_seq_cst)
			{
				enter_critical();
				this->_value = v;
				exit_critical();
			}

			void store(T v, memory_order order = memory_order_seq_cst) volatile
			{
				enter_critical();
				this->_value = v;
				exit_critical();
			}

			bool operator==(const Atomic<T> &other)
			{
				enter_critical();
				auto ours = this->_value;
				exit_critical();

				return ours == other.value();
			}

			bool operator!=(const Atomic<T> &other)
			{
				enter_critical();
				auto ours = this->_value;
				exit_critical();

				return ours != other.value();
			}

			T operator++()
			{
				return fetch_add(1) + 1;
			}

			T operator++() volatile
			{
				return fetch_add(1) + 1;
			}

			// Post-increment
			T operator++(int)
			{
				return fetch_add(1);
			}

			T operator++(int) volatile
			{
				return fetch_add(1);
			}

			T operator--()
			{
				return fetch_sub(1) + 1;
			}

			T operator--() volatile
			{
				return fetch_sub(1) + 1;
			}

			T operator--(int)
			{
				return fetch_sub(1);
			}

			T operator--(int) volatile
			{
				return fetch_sub(1);
			}

			T operator+=(T v)
			{
				return fetch_add(v) + v;
			}

			T operator+=(T v) volatile
			{
				return fetch_add(v) + v;
			}

			T operator-=(T v)
			{
				return fetch_sub(v) - v;
			}

			T operator-=(T v) volatile
			{
				return fetch_sub(v) - v;
			}

			// And
			T operator&=(T v)
			{
				return fetch_and(v) & v;
			}

			T operator&=(T v) volatile
			{
				return fetch_and(v) & v;
			}

			T operator|=(T v)
			{
				return fetch_or(v) | v;
			}

			T operator|=(T v) volatile
			{
				return fetch_or(v) | v;
			}

			T operator^=(T v)
			{
				return fetch_xor(v) ^ v;
			}

			T operator^=(T v) volatile
			{
				return fetch_xor(v) ^ v;
			}

		private:
			T _value;
		};
	}

	using memory_order = detail::memory_order;
	template <typename T>
	using Atomic = detail::Atomic<T>;
}
