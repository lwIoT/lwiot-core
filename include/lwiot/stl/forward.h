/*
 * Reference forwarding.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/**
 * @file forward.h Forward header.
 */

/// @addtogroup stl
/// @{

#pragma once

#include <lwiot/traits/removereference.h>
#include <lwiot/traits/isreference.h>

namespace lwiot
{
	namespace stl
	{
		/**
		 * @brief Forward \p __t as an lvalue or rvalue reference based on \p _Tp.
		 * @tparam _Tp Type to be forwarded.
		 * @param __t Value to be forwarded.
		 * @return An lvalue or rvalue reference, depending on \p __t.
		 */
		template<typename _Tp>
		constexpr _Tp &&forward(typename traits::RemoveReference<_Tp>::type &__t) noexcept
		{
			return static_cast<_Tp &&>(__t);
		}

		/**
		 * @brief Forward the rvalue reference \p t.
		 * @tparam T Type to be forwarded.
		 * @param t Value to be forwarded.
		 * @return An rvalue reference to \p t.
		 */
		template<typename T>
		constexpr T &&forward(typename traits::RemoveReference<T>::type &&t) noexcept
		{
			static_assert(!traits::IsLValueReference<T>::value,
			              "template argument substituting T is an lvalue reference type");
			return static_cast<T &&>(t);
		}
	}
}

/// @}
