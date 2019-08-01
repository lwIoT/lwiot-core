/*
 * Move data by value.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/stl/forward.h>
#include <lwiot/traits/removereference.h>
#include <lwiot/traits/removecv.h>
#include <lwiot/traits/enableif.h>

namespace lwiot {
	namespace stl {
		template<typename _Tp>
		constexpr typename traits::RemoveReference<_Tp>::type&& move(_Tp&& __t) noexcept
		{
			return static_cast<typename traits::RemoveReference<_Tp>::type &&>(__t);
		}

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

		template <typename T>
		constexpr void swap(T& a, T& b)
		{
			b = move(exchange(a, move(b)));
		}
} }
