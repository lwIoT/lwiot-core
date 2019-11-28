/*
 * Move data by value.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

/// @file move.h Move utilities.

#include <lwiot/stl/forward.h>
#include <lwiot/traits/removereference.h>
#include <lwiot/traits/removecv.h>
#include <lwiot/traits/enableif.h>

namespace lwiot {
	namespace stl {
		/**
		 * @ingroup stl
		 * @brief Indicate that an object may be "moved from", i.e. allow efficient transfer of resources from \p __t to
		 *        another object.
		 * @tparam _Tp Type to move.
		 * @param __t Object to be moved.
		 * @return Rvalue reference to \p __t.
		 */
		template<typename _Tp>
		constexpr typename traits::RemoveReference<_Tp>::type&& move(_Tp&& __t) noexcept
		{
			return static_cast<typename traits::RemoveReference<_Tp>::type &&>(__t);
		}

		/**
		 * @ingroup stl
		 * @brief Exchange the values of \p obj and \p nobj.
		 * @tparam T Object type.
		 * @param obj Target object.
		 * @param nobj Object to move into \p obj.
		 * @return The old value of \p obj.
		 */
		template <typename T>
		constexpr T exchange(T& obj, T&& nobj)
		{
			T old = move(obj);
			obj = forward<T>(nobj);
			return old;
		}

		/*template <typename T>
		constexpr void swap(T& a, T& b)
		{
			auto old = move(a);

			a = stl::move(b);
			b = stl::move(old);
		}*/

		/**
		 * @ingroup stl
		 * @brief Swap two objects.
		 * @tparam T Object type.
		 * @param a First object.
		 * @param b Second object.
		 */
		template <typename T>
		constexpr void swap(T& a, T& b)
		{
			b = move(exchange(a, move(b)));
		}
} }
