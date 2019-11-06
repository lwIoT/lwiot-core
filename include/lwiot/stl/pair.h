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

			Pair( first_type&& a,  second_type&& b) :
				first(stl::forward<first_type>(a)), second(stl::forward<second_type>(b))

			{

			}

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

			friend constexpr void swap(Pair& a, Pair& b)
			{
				using stl::swap;

				swap(a.first, b.first);
				swap(a.second, b.second);
			}

			first_type first;
			second_type second;
		};
	}
}
