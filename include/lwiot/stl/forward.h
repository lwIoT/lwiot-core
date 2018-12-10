/*
 * Reference forwarding.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/traits/removereference.h>
#include <lwiot/traits/isreference.h>

namespace lwiot { namespace stl {
	template<typename _Tp>
	constexpr _Tp&&
	forward(typename traits::RemoveReference<_Tp>::type& __t) noexcept
	{
		return static_cast<_Tp &&>(__t);
	}

	template <typename T>
	constexpr T&& forward(typename traits::RemoveReference<T>::type&& t) noexcept
	{
		static_assert(!traits::IsLValueReference<T>::value, "template argument substituting T is an lvalue reference type");
		return static_cast<T&&>(t);
	}
}}
