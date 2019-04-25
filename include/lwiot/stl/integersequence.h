/*
 * Integer sequence header.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <stddef.h>

namespace lwiot
{
	namespace stl
	{
		template <typename T, T... idx>
		struct IntegerSequence {
			typedef T ValueType;
			static constexpr size_t size() noexcept
			{
				return sizeof...(idx);
			}

			using type = IntegerSequence<T, idx...>;
		};
	}
}
