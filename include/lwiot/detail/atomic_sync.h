/*
 * GCC sync atomic implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/**
 * @file atomic_sync.h Atomic header.
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
		/**
		 * @brief Memory ordering.
		 * @ingroup kernel
		 */
		typedef enum {
			memory_order_relaxed, //!< Relaxed operation.
			memory_order_consume, //!< Loads perform a consume operation.
			memory_order_acquire, //!< Loads perform an acquire operation.
			memory_order_release, //!< Stores perform a release operation.
			memory_order_acq_rel, //!< An RW-op performs both an acquire and a release operation.
			memory_order_seq_cst //!< Combination of memory_order_acquire, memory_order_release and memory_order_acq_rel.
		} memory_order;

		/**
		 * @ingroup kernel
		 * @brief Atomic type definition.
		 * @tparam T Value type that is bound to an instance of \p Atomic.
		 */
		template <typename T>
		class Atomic {
		public:
			static_assert(traits::IsIntegral<T>::value, "Atomic only works for integral types!");

			/**
			 * @brief Create a new instance of an atomic type.
			 */
			constexpr Atomic() : _value() { }

			/**
			 * @brief Construct a new atomic type and initialize the value with \p value.
			 * @param value Value to use as initial value.
			 */
			explicit constexpr Atomic(T value) : _value(value) { }

			/**
			 * @brief Copy constructor.
			 * @param value Atomic object to copy into \p *this.
			 */
			Atomic(const Atomic& value) : _value(value.load())
			{ }

			/**
			 * @brief Move constructor.
			 * @param value Atomic object to move into \p *this.
			 */
			Atomic(Atomic&& value) noexcept : _value(value.load())
			{ }

			/**
			 * @brief Copy assignment operator.
			 * @param value Atomic object to move into \p *this.
			 * @return A reference to *this.
			 */
			constexpr Atomic& operator=(const Atomic& value)
			{
				this->store(value.load());
				return *this;
			}

			/**
			 * @brief Move assignment operator.
			 * @param value Atomic object to move into \p *this.
			 * @return A reference to *this.
			 */
			constexpr Atomic& operator=(Atomic&& value) noexcept
			{
				this->store(value.load());
				return *this;
			}

			/**
			 * @brief Store a new value into \p *this.
			 * @param value Value to store into \p *this.
			 * @return Reference to \p *this.
			 */
			constexpr Atomic& operator = (const T& value) noexcept
			{
				this->store(value);
				return *this;
			}

			/**
			 * @brief Store a new value into \p *this.
			 * @param value Value to store into \p *this.
			 * @return Reference to \p *this.
			 */
			constexpr Atomic& operator = (const T& value) volatile noexcept
			{
				this->store(value);
				return *this;
			}

			/**
			 * Pre increment operator.
			 * @return \p *this as value.
			 */
			T operator ++()
			{
				return fetch_add(1) + 1;
			}

			/**
			 * Pre increment operator.
			 * @return \p *this as value.
			 */
			T operator ++() volatile
			{
				return fetch_add(1) + 1;
			}

			/**
			 * Pre increment operator.
			 * @return Reference to \p *this.
			 */
			T operator ++(int)
			{
				return fetch_add(1);
			}

			/**
			 * Pre increment operator.
			 * @return Reference to \p *this.
			 */
			T operator ++(int) volatile
			{
				return fetch_add(1);
			}

			/**
			 * Pre decrement operator.
			 * @return \p *this as value.
			 */
			T operator --()
			{
				return fetch_sub(1) + 1;
			}

			/**
			 * Pre decrement operator.
			 * @return \p *this as value.
			 */
			T operator --() volatile
			{
				return fetch_sub(1) + 1;
			}

			/**
			 * Post decrement operator.
			 * @return \p *this as value.
			 */
			T operator --(int)
			{
				return fetch_sub(1);
			}

			/**
			 * Post decrement operator.
			 * @return \p *this as value.
			 */
			T operator --(int) volatile
			{
				return fetch_sub(1);
			}

			/**
			 * @brief Atomic computation operator.
			 * @param v Argument to the computation.
			 * @return \p *this as a value.
			 *
			 * Atomically replaces the current value with the result of computation
			 * involving the previous value and arg. The operation is read-modify-write
			 * operation.
			 */
			T operator +=(T v)
			{
				return fetch_add(v) + v;
			}

			/**
			 * @brief Atomic computation operator.
			 * @param v Argument to the computation.
			 * @return \p *this as a value.
			 *
			 * Atomically replaces the current value with the result of computation
			 * involving the previous value and arg. The operation is read-modify-write
			 * operation.
			 */
			T operator +=(T v) volatile
			{
				return fetch_add(v) + v;
			}

			/**
			 * @brief Atomic computation operator.
			 * @param v Argument to the computation.
			 * @return \p *this as a value.
			 *
			 * Atomically replaces the current value with the result of computation
			 * involving the previous value and arg. The operation is read-modify-write
			 * operation.
			 */
			T operator -=(T v)
			{
				return fetch_sub(v) - v;
			}

			/**
			 * @brief Atomic computation operator.
			 * @param v Argument to the computation.
			 * @return \p *this as a value.
			 *
			 * Atomically replaces the current value with the result of computation
			 * involving the previous value and arg. The operation is read-modify-write
			 * operation.
			 */
			T operator -=(T v) volatile
			{
				return fetch_sub(v) - v;
			}

			/**
			 * @brief Atomic computation operator.
			 * @param v Argument to the computation.
			 * @return \p *this as a value.
			 *
			 * Atomically replaces the current value with the result of computation
			 * involving the previous value and arg. The operation is read-modify-write
			 * operation.
			 */
			T operator &=(T v)
			{
				return fetch_and(v) & v;
			}

			T operator &=(T v) volatile
			{
				return fetch_and(v) & v;
			}

			/**
			 * @brief Atomic computation operator.
			 * @param v Argument to the computation.
			 * @return \p *this as a value.
			 *
			 * Atomically replaces the current value with the result of computation
			 * involving the previous value and arg. The operation is read-modify-write
			 * operation.
			 */
			T operator |=(T v)
			{
				return fetch_or(v) | v;
			}

			/**
			 * @brief Atomic computation operator.
			 * @param v Argument to the computation.
			 * @return \p *this as a value.
			 *
			 * Atomically replaces the current value with the result of computation
			 * involving the previous value and arg. The operation is read-modify-write
			 * operation.
			 */
			T operator |=(T v) volatile
			{
				return fetch_or(v) | v;
			}

			/**
			 * @brief Atomic computation operator.
			 * @param v Argument to the computation.
			 * @return \p *this as a value.
			 *
			 * Atomically replaces the current value with the result of computation
			 * involving the previous value and arg. The operation is read-modify-write
			 * operation.
			 */
			T operator ^=(T v)
			{
				return fetch_xor(v) ^ v;
			}

			/**
			 * @brief Atomic computation operator.
			 * @param v Argument to the computation.
			 * @return \p *this as a value.
			 *
			 * Atomically replaces the current value with the result of computation
			 * involving the previous value and arg. The operation is read-modify-write
			 * operation.
			 */
			T operator ^=(T v) volatile
			{
				return fetch_xor(v) ^ v;
			}

			/**
			 * @brief Conversion operator.
			 * @return The value wrapper by \p *this.
			 */
			explicit operator T () const
			{
				return __sync_fetch_and_add((T*) &this->_value, 0);
			}

			/**
			 * @brief Conversion operator.
			 * @return The value wrapper by \p *this.
			 */
			explicit operator T() volatile const
			{
				return __sync_fetch_and_add((T*) &this->_value, 0);
			}

			/**
			 * @brief Check if the implementation of \p atomic is lock free.
			 * @return True or false based on whether or not this the implementation is lock free.
			 */
			bool is_lock_free() const
			{
				return true;
			}

			/**
			 * @brief Check if the implementation of \p atomic is lock free.
			 * @return True or false based on whether or not this the implementation is lock free.
			 */
			bool is_lock_free() const volatile
			{
				return true;
			}

			/**
			 * @brief Store operation.
			 * @param v Value to store.
			 * @param order Memory ordering.
			 */
			void store(T v, detail::memory_order order = detail::memory_order_seq_cst)
			{
				__sync_lock_test_and_set(&this->_value, v);
			}

			/**
			 * @brief Store operation.
			 * @param v Value to store.
			 * @param order Memory ordering.
			 */
			void store(T v, detail::memory_order order = detail::memory_order_seq_cst) volatile
			{
				__sync_lock_test_and_set(&this->_value, v);
			}

			/**
			 * @brief Load operation.
			 * @param order Memory ordering.
			 * @return The value stored by \p *this.
			 */
			T load(detail::memory_order order = detail::memory_order_seq_cst) const
			{
				return __sync_fetch_and_add((volatile T*) &this->_value, 0);
			}

			/**
			 * @brief Load operation.
			 * @param order Memory ordering.
			 * @return The value stored by \p *this.
			 */
			T load(detail::memory_order order = detail::memory_order_seq_cst) const volatile
			{
				return __sync_fetch_and_add(&this->_value, 0);
			}

			/**
			 * @brief Atomically adds the argument to the value stored in the atomic object and obtains
			 *        the value held previously
			 * @param v Value to add.
			 * @param order Memory order.
			 * @return The value held by \p *this.
			 */
			T fetch_add(T v, detail::memory_order order = detail::memory_order_seq_cst)
			{
				return __sync_fetch_and_add(&this->_value, v);
			}

			/**
			 * @brief Atomically adds the argument to the value stored in the atomic object and obtains
			 *        the value held previously
			 * @param v Value to add.
			 * @param order Memory order.
			 * @return The value held by \p *this.
			 */
			T fetch_add(T v, detail::memory_order order = detail::memory_order_seq_cst) volatile
			{
				return __sync_fetch_and_add(&this->_value, v);
			}

			/**
			 * @brief Atomically substracts the argument to the value stored in the atomic object and obtains
			 *        the value held previously
			 * @param v Value to substract.
			 * @param order Memory order.
			 * @return The value held by \p *this.
			 */
			T fetch_sub(T v, detail::memory_order order = detail::memory_order_seq_cst)
			{
				return __sync_fetch_and_sub(&this->_value, v);
			}

			/**
			 * @brief Atomically substracts the argument to the value stored in the atomic object and obtains
			 *        the value held previously
			 * @param v Value to substract.
			 * @param order Memory order.
			 * @return The value held by \p *this.
			 */
			T fetch_sub(T v, detail::memory_order order = detail::memory_order_seq_cst) volatile
			{
				return __sync_fetch_and_sub(&this->_value, v);
			}

			/**
			 * @brief Atomically performs a bitwise OR with \p v and obtains the value held previously.
			 * @param v Value object.
			 * @param order Memory order.
			 * @return The value held by \p *this.
			 */
			T fetch_or(T v, detail::memory_order order = detail::memory_order_seq_cst)
			{
				return __sync_fetch_and_or(&this->_value, v);
			}

			/**
			 * @brief Atomically performs a bitwise OR with \p v and obtains the value held previously.
			 * @param v Value object.
			 * @param order Memory order.
			 * @return The value held by \p *this.
			 */
			T fetch_or(T v, detail::memory_order order = detail::memory_order_seq_cst) volatile
			{
				return __sync_fetch_and_or(&this->_value, v);
			}

			/**
			 * @brief Atomically performs a bitwise AND with \p v and obtains the value held previously.
			 * @param v Value object.
			 * @param order Memory order.
			 * @return The value held by \p *this.
			 */
			T fetch_and(T v, detail::memory_order order = detail::memory_order_seq_cst)
			{
				return __sync_fetch_and_and(&this->_value, v);
			}

			/**
			 * @brief Atomically performs a bitwise AND with \p v and obtains the value held previously.
			 * @param v Value object.
			 * @param order Memory order.
			 * @return The value held by \p *this.
			 */
			T fetch_and(T v, detail::memory_order order = detail::memory_order_seq_cst) volatile
			{
				return __sync_fetch_and_and(&this->_value, v);
			}

			/**
			 * @brief Atomically performs a bitwise XOR with \p v and obtains the value held previously.
			 * @param v Value object.
			 * @param order Memory order.
			 * @return The value held by \p *this.
			 */
			T fetch_xor(T v, detail::memory_order order = detail::memory_order_seq_cst)
			{
				return __sync_fetch_and_xor(&this->_value, v);
			}

			/**
			 * @brief Atomically performs a bitwise XOR with \p v and obtains the value held previously.
			 * @param v Value object.
			 * @param order Memory order.
			 * @return The value held by \p *this.
			 */
			T fetch_xor(T v, detail::memory_order order = detail::memory_order_seq_cst) volatile
			{
				return __sync_fetch_and_xor(&this->_value, v);
			}

			/**
			 * @brief exchange Atomically replace the value with \p v and obtains the value held previously.
			 * @param v Value to exchange.
			 * @param order Memory order.
			 * @return The value held by \p *this.
			 */
			T exchange(T v, detail::memory_order order = detail::memory_order_seq_cst)
			{
				return __sync_lock_test_and_set(&this->_value, v);
			}

			/**
			 * @brief exchange Atomically replace the value with \p v and obtains the value held previously.
			 * @param v Value to exchange.
			 * @param order Memory order.
			 * @return The value held by \p *this.
			 */
			T exchange(T v, detail::memory_order order = detail::memory_order_seq_cst) volatile
			{
				return __sync_lock_test_and_set(&this->_value, v);
			}

			/**
			 * @brief exchange Atomically compare the object representation of \* this with \p expected.
			 * @param expected Expected value.
			 * @param desired Value to store after comparison.
			 * @param order Memory order.
			 * @return The value held by \p *this.
			 */
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

			/**
			 * @brief exchange Atomically compare the object representation of \* this with \p expected.
			 * @param expected Expected value.
			 * @param desired Value to store after comparison.
			 * @param order Memory order.
			 * @return The value held by \p *this.
			 * @note This function is allowed to fail spuriously.
			 */
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

			/**
			 * @brief exchange Atomically compare the object representation of \* this with \p expected.
			 * @param expected Expected value.
			 * @param desired Value to store after comparison.
			 * @param success Memory order for the RW operation when the comparison succeeds.
			 * @param failure Memory order for the load operation if the comparison fails.
			 * @return The value held by \p *this.
			 * @note This function is allowed to fail spuriously.
			 */
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

			/**
			 * @brief exchange Atomically compare the object representation of \* this with \p expected.
			 * @param expected Expected value.
			 * @param desired Value to store after comparison.
			 * @param success Memory order for the RW operation when the comparison succeeds.
			 * @param failure Memory order for the load operation if the comparison fails.
			 * @return The value held by \p *this.
			 * @note This function is allowed to fail spuriously.
			 */
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

			/**
			 * @brief exchange Atomically compare the object representation of \* this with \p expected.
			 * @param expected Expected value.
			 * @param desired Value to store after comparison.
			 * @param order Memory order.
			 * @return The value held by \p *this.
			 */
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

			/**
			 * @brief exchange Atomically compare the object representation of \* this with \p expected.
			 * @param expected Expected value.
			 * @param desired Value to store after comparison.
			 * @param order Memory order.
			 * @return The value held by \p *this.
			 */
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

			/**
			 * @brief exchange Atomically compare the object representation of \* this with \p expected.
			 * @param expected Expected value.
			 * @param desired Value to store after comparison.
			 * @param success Memory order for the RW operation when the comparison succeeds.
			 * @param failure Memory order for the load operation if the comparison fails.
			 * @return The value held by \p *this.
			 */
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

			/**
			 * @brief exchange Atomically compare the object representation of \* this with \p expected.
			 * @param expected Expected value.
			 * @param desired Value to store after comparison.
			 * @param success Memory order for the RW operation when the comparison succeeds.
			 * @param failure Memory order for the load operation if the comparison fails.
			 * @return The value held by \p *this.
			 */
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
