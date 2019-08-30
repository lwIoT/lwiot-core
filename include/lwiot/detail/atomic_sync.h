/*
 * GCC sync atomic implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/traits/isintegral.h>

#ifndef HAVE_SYNC_FETCH

#include "fetch_sync.h"
#endif

namespace lwiot
{
	namespace detail
	{
		typedef enum memory_order
		{
			memory_order_relaxed,
			memory_order_consume,
			memory_order_acquire,
			memory_order_release,
			memory_order_acq_rel,
			memory_order_seq_cst
		} memory_order;

		template <typename T>
		class Atomic {
		public:
			static_assert(traits::IsIntegral<T>::value, "Atomic only works for integral types!");

			constexpr Atomic() : _value() { }

			explicit constexpr Atomic(T value) : _value(value) { }

			Atomic(const Atomic& value) : _value(value.load())
			{ }

			Atomic(Atomic&& value) noexcept : _value(value.load())
			{ }

			constexpr Atomic& operator=(const Atomic& value)
			{
				this->store(value.load());
				return *this;
			}

			constexpr Atomic& operator=(Atomic&& value) noexcept
			{
				this->store(value.load());
				return *this;
			}

			constexpr Atomic& operator = (const T& value) noexcept
			{
				this->store(value);
				return *this;
			}

			constexpr Atomic& operator = (const T& value) volatile noexcept
			{
				this->store(value);
				return *this;
			}

			T operator ++()
			{
				return fetch_add(1) + 1;
			}

			T operator ++() volatile
			{
				return fetch_add(1) + 1;
			}

			// Post-increment
			const T operator ++(int)
			{
				return fetch_add(1);
			}

			const T operator ++(int) volatile
			{
				return fetch_add(1);
			}

			// Pre-decrement
			T operator --()
			{
				return fetch_sub(1) + 1;
			}

			T operator --() volatile
			{
				return fetch_sub(1) + 1;
			}

			// Post-decrement
			const T operator --(int)
			{
				return fetch_sub(1);
			}

			const T operator --(int) volatile
			{
				return fetch_sub(1);
			}

			// Add
			T operator +=(T v)
			{
				return fetch_add(v) + v;
			}

			T operator +=(T v) volatile
			{
				return fetch_add(v) + v;
			}

			// Subtract
			T operator -=(T v)
			{
				return fetch_sub(v) - v;
			}

			T operator -=(T v) volatile
			{
				return fetch_sub(v) - v;
			}

			// And
			T operator &=(T v)
			{
				return fetch_and(v) & v;
			}

			T operator &=(T v) volatile
			{
				return fetch_and(v) & v;
			}

			// Or
			T operator |=(T v)
			{
				return fetch_or(v) | v;
			}

			T operator |=(T v) volatile
			{
				return fetch_or(v) | v;
			}

			// Exclusive or
			T operator ^=(T v)
			{
				return fetch_xor(v) ^ v;
			}

			T operator ^=(T v) volatile
			{
				return fetch_xor(v) ^ v;
			}

			// Conversion operator
			explicit operator T () const
			{
				return __sync_fetch_and_add((T*) &this->_value, 0);
			}

			explicit operator T() volatile const
			{
				return __sync_fetch_and_add((T*) &this->_value, 0);
			}

			// Is lock free?
			bool is_lock_free() const
			{
				return true;
			}

			bool is_lock_free() const volatile
			{
				return true;
			}

			// Store
			void store(T v, detail::memory_order order = detail::memory_order_seq_cst)
			{
				__sync_lock_test_and_set(&this->_value, v);
			}

			void store(T v, detail::memory_order order = detail::memory_order_seq_cst) volatile
			{
				__sync_lock_test_and_set(&this->_value, v);
			}

			// Load
			T load(detail::memory_order order = detail::memory_order_seq_cst) const
			{
				return __sync_fetch_and_add((volatile T*) &this->_value, 0);
			}

			T load(detail::memory_order order = detail::memory_order_seq_cst) const volatile
			{
				return __sync_fetch_and_add(&this->_value, 0);
			}

			// Fetch add
			T fetch_add(T v, detail::memory_order order = detail::memory_order_seq_cst)
			{
				return __sync_fetch_and_add(&this->_value, v);
			}

			T fetch_add(T v, detail::memory_order order = detail::memory_order_seq_cst) volatile
			{
				return __sync_fetch_and_add(&this->_value, v);
			}

			// Fetch subtract
			T fetch_sub(T v, detail::memory_order order = detail::memory_order_seq_cst)
			{
				return __sync_fetch_and_sub(&this->_value, v);
			}

			T fetch_sub(T v, detail::memory_order order = detail::memory_order_seq_cst) volatile
			{
				return __sync_fetch_and_sub(&this->_value, v);
			}

			// Fetch or
			T fetch_or(T v, detail::memory_order order = detail::memory_order_seq_cst)
			{
				return __sync_fetch_and_or(&this->_value, v);
			}

			T fetch_or(T v, detail::memory_order order = detail::memory_order_seq_cst) volatile
			{
				return __sync_fetch_and_or(&this->_value, v);
			}

			// Fetch and
			T fetch_and(T v, detail::memory_order order = detail::memory_order_seq_cst)
			{
				return __sync_fetch_and_and(&this->_value, v);
			}

			T fetch_and(T v, detail::memory_order order = detail::memory_order_seq_cst) volatile
			{
				return __sync_fetch_and_and(&this->_value, v);
			}

			// Fetch exclusive or
			T fetch_xor(T v, detail::memory_order order = detail::memory_order_seq_cst)
			{
				return __sync_fetch_and_xor(&this->_value, v);
			}

			T fetch_xor(T v, detail::memory_order order = detail::memory_order_seq_cst) volatile
			{
				return __sync_fetch_and_xor(&this->_value, v);
			}

			// Exchange
			T exchange(T v, detail::memory_order order = detail::memory_order_seq_cst)
			{
				return __sync_lock_test_and_set(&this->_value, v);
			}

			T exchange(T v, detail::memory_order order = detail::memory_order_seq_cst) volatile
			{
				return __sync_lock_test_and_set(&this->_value, v);
			}

			// Compare exchange weak
			bool compare_exchange_weak(T& expected, T desired, detail::memory_order order = detail::memory_order_seq_cst)
			{
				T old = __sync_val_compare_and_swap(&this->_value, expected, desired);

				if (old == expected)
				{
					return true;
				}
				else
				{
					expected = old;
					return false;
				}
			}

			bool compare_exchange_weak(T& expected, T desired, detail::memory_order order = detail::memory_order_seq_cst) volatile
			{
				T old = __sync_val_compare_and_swap(&this->_value, expected, desired);

				if (old == expected)
				{
					return true;
				}
				else
				{
					expected = old;
					return false;
				}
			}

			bool compare_exchange_weak(T& expected, T desired, detail::memory_order success, detail::memory_order failure)
			{
				T old = __sync_val_compare_and_swap(&this->_value, expected, desired);

				if (old == expected)
				{
					return true;
				}
				else
				{
					expected = old;
					return false;
				}
			}

			bool compare_exchange_weak(T& expected, T desired, detail::memory_order success, detail::memory_order failure) volatile
			{
				T old = __sync_val_compare_and_swap(&this->_value, expected, desired);

				if (old == expected)
				{
					return true;
				}
				else
				{
					expected = old;
					return false;
				}
			}

			// Compare exchange strong
			bool compare_exchange_strong(T& expected, T desired, detail::memory_order order = detail::memory_order_seq_cst)
			{
				T old = expected;

				while (!compare_exchange_weak(old, desired))
				{
					if (memcmp(&old, &expected, sizeof(T)))
					{
						expected = old;
						return false;
					}
				}

				return true;
			}

			bool compare_exchange_strong(T& expected, T desired, detail::memory_order order = detail::memory_order_seq_cst) volatile
			{
				T old = expected;

				while (!compare_exchange_weak(old, desired))
				{
					if (memcmp(&old, &expected, sizeof(T)))
					{
						expected = old;
						return false;
					}
				}

				return true;
			}

			bool compare_exchange_strong(T& expected, T desired, detail::memory_order success, detail::memory_order failure)
			{
				T old = expected;

				while (!compare_exchange_weak(old, desired))
				{
					if (memcmp(&old, &expected, sizeof(T)))
					{
						expected = old;
						return false;
					}
				}

				return true;
			}

			bool compare_exchange_strong(T& expected, T desired, detail::memory_order success, detail::memory_order failure) volatile
			{
				T old = expected;

				while (!compare_exchange_weak(old, desired))
				{
					if (memcmp(&old, &expected, sizeof(T)))
					{
						expected = old;
						return false;
					}
				}

				return true;
			}

		private:
			T _value;
		};
	}

	using memory_order = detail::memory_order;
	template <typename T>
	using Atomic = detail::Atomic<T>;
}
