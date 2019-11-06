/*
 * Check if a type is copy constructible.
 *
 * @author Michel Megens
 * @email  michel@michelmegens.net
 */

#pragma once

#include <lwiot/traits/isreference.h>
#include <lwiot/traits/integralconstant.h>

namespace lwiot
{
	namespace traits
	{
		template<typename _Tp, bool = IsReferenceable<_Tp>::value>
		struct IsCopyConstructible_impl;

		template<typename _Tp>
		struct IsCopyConstructible_impl<_Tp, false>
				: public FalseType { };

		template<typename _Tp>
		struct IsCopyConstructible_impl<_Tp, true>
				: public IsConstructible<_Tp, const _Tp&>
		{ };

		template<typename _Tp>
		struct IsCopyConstructible : public IsCopyConstructible_impl<_Tp>
		{ };
	}
}
