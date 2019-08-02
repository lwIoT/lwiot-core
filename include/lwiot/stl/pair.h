/*
 * STL pair implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <lwiot.h>

namespace lwiot
{
	namespace stl
	{
		template <typename X, typename Y>
		struct Pair {
			typedef X first_type;
			typedef Y second_type;

			constexpr explicit Pair() : first(), second()
			{}

			Pair(const first_type& a, const second_type& b) : first(a), second(b)
			{}

			constexpr Pair(const Pair& p) = default;
			constexpr Pair(Pair&& p) noexcept = default;

			Pair& operator=(const Pair& p)
			{
				this->first = p.first;
				this->second = p.second;
				return *this;
			}

			Pair& operator=(Pair&& p) noexcept
			{
				this->first = stl::forward<first_type>(p.first);
				this->second = stl::forward<second_type>(p.second);
				return *this;
			}

			constexpr void swap(Pair& p)
			{
				using stl::swap;
				swap(this->first, p.first);
				swap(this->second, p.second);
			}

			first_type first;
			second_type second;
		};
	}
}
