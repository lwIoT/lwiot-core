/*
 * STL pair implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file pair.h Pair header.

#pragma once

#include <stdlib.h>
#include <lwiot.h>

namespace lwiot
{
	namespace stl
	{
		/**
		 * @ingroup stl
		 * @brief Pair container.
		 * @tparam X First type.
		 * @tparam Y Second type.
		 *
		 * Store two heterogeneous objects as a single unit.
		 */
		template <typename X, typename Y>
		struct Pair {
			typedef X first_type; //!< First type.
			typedef Y second_type; //!< Second type.

			/**
			 * @brief Construct a new pair object.
			 */
			constexpr explicit Pair() : first(), second()
			{}

			/**
			 * @brief Construct a new pair object.
			 * @param a First value.
			 * @param b Second value.
			 */
			Pair(const first_type& a, const second_type& b) : first(a), second(b)
			{}

			/**
			 * @brief Construct a new pair object.
			 * @param a First value.
			 * @param b Second value.
			 * @note This constructor utilizes move construction.
			 */
			Pair( first_type&& a,  second_type&& b) :
				first(stl::forward<first_type>(a)), second(stl::forward<second_type>(b))

			{

			}

			constexpr Pair(const Pair& p) = default;
			constexpr Pair(Pair&& p) noexcept = default;

			Pair& operator=(const Pair& p) noexcept = default;
			Pair& operator=(Pair&& p) noexcept = default;

			/**
			 * @brief Swap two pair objects.
			 * @param a First pair object.
			 * @param b Second pair object.
			 */
			friend constexpr void swap(Pair& a, Pair& b)
			{
				using stl::swap;

				swap(a.first, b.first);
				swap(a.second, b.second);
			}

			first_type first; //!< First object.
			second_type second; //!< Second object.
		};
	}
}
