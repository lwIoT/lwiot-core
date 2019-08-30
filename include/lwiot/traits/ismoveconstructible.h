/*
 * Move construction type trait.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/traits/isreference.h>
#include <lwiot/traits/integralconstant.h>
#include <lwiot/traits/isconstructible.h>

namespace lwiot
{
	namespace traits
	{
		template<typename _Tp, bool = IsReferenceable<_Tp>::value>
		struct __is_move_constructible_impl;

		template<typename _Tp>
		struct __is_move_constructible_impl<_Tp, false> : public FalseType
		{ };

		template<typename _Tp>
		struct __is_move_constructible_impl<_Tp, true> : public IsConstructible<_Tp, _Tp&&>
		{ };

		template<typename _Tp>
		struct IsMoveConstructible : public __is_move_constructible_impl<_Tp>
		{ };
	}
}