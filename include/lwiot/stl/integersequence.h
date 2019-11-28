/*
 * Integer sequence header.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

/**
 * @file integersequence.h
 */

#include <stdlib.h>
#include <stddef.h>

namespace lwiot
{
	namespace stl
	{
		/**
		 * @ingroup stl
		 * @brief Compile time representation of a sequence of integers.
		 * @tparam T Integer type.
		 * @tparam T... Sequence of integers.
		 */
		template <typename T, T... idx>
		struct IntegerSequence {
			typedef T ValueType; //!< Value type.

			/**
			 * @brief Get the size of the integer sequence.
			 * @return The size of \p *this;
			 */
			static constexpr size_t size() noexcept
			{
				return sizeof...(idx);
			}

			/**
			 * @brief Sequence type.
			 */
			using type = IntegerSequence<T, idx...>;
		};
	}
}
