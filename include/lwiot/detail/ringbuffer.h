/*
 * Ring buffer base implementation.
 *
 * @author Michel Megens
 * @email  michel@michelmegens.net
 */

#pragma once

#include <string.h>

#include <lwiot/stl/array.h>
#include <lwiot/stl/move.h>

#include <lwiot/traits/isconstructible.h>

namespace lwiot
{
	namespace detail_rb
	{
		template <typename T, stl::size_t S>
		class RingBuffer {
			static constexpr stl::size_t BufferSize = S;

			static_assert(traits::IsDefaultConstructible<T>::value, "Argument T should be default constructible!");

		public:
			typedef T ValueType;

			RingBuffer() : m_head(0UL), m_tail(0UL)
			{
				memset(this->m_buffer.data(), 0, BufferSize);
			}

			constexpr stl::size_t size()
			{
				return BufferSize;
			}

			void store(ValueType&& value)
			{
				auto nxt = this->index(this->m_head);

				if(nxt != this->m_tail) {
					this->m_buffer[this->m_head] = stl::move(value);
					this->m_head = nxt;
				}
			}

			void store(const ValueType& value)
			{
				auto nxt = this->index(this->m_head);

				if(nxt != this->m_tail) {
					this->m_buffer[this->m_head] = value;
					this->m_head = nxt;
				}
			}

			ValueType read()
			{
				if(this->m_tail == this->m_head)
					return ValueType();

				auto tmp = this->m_tail;
				this->m_tail = this->index(this->m_tail);

				return this->m_buffer.at(tmp);
			}

			ValueType peek() const
			{
				if(this->m_tail == this->m_head)
					return ValueType();

				return this->m_buffer.at(this->m_tail);
			}

			stl::size_t available()
			{
				stl::size_t  rv;
				long delta = this->m_head - this->m_tail;

				if(delta < 0) {
					rv = BufferSize + delta;
				} else {
					rv = delta;
				}

				return rv;
			}

			stl::size_t availableForWrite()
			{
				if (this->m_head >= this->m_tail)
					return BufferSize - 1 - this->m_head + this->m_tail;
				else
					return this->m_tail - this->m_head - 1;
			}

			void clear()
			{
				this->m_head = 0UL;
				this->m_tail = 0UL;
			}

		private:
			stl::Array<ValueType, BufferSize> m_buffer;
			stl::size_t m_head;
			stl::size_t m_tail;

			/* Methods */
			stl::size_t index(stl::size_t idx) const
			{
				return static_cast<stl::size_t>((idx + 1) % BufferSize);
			}
		};
	}
}
