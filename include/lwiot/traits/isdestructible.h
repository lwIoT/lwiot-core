/*
 * Destructible type trait.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/traits/integralconstant.h>
#include <lwiot/traits/isreference.h>
#include <lwiot/traits/isarray.h>
#include <lwiot/traits/isscalar.h>

namespace lwiot
{
	namespace traits
	{
		template<typename>
		struct RemoveAllExtents;

		template<typename _Tp>
		struct RemoveAllExtents
		{ typedef _Tp     type; };

		template<typename _Tp, std::size_t _Size>
		struct RemoveAllExtents<_Tp[_Size]>
		{ typedef typename RemoveAllExtents<_Tp>::type     type; };

		template<typename _Tp>
		struct RemoveAllExtents<_Tp[]>
		{ typedef typename RemoveAllExtents<_Tp>::type     type; };

		template<typename, unsigned = 0>
		struct extent;

		template<typename, unsigned _Uint>
		struct extent : public IntegralConstant<size_t, 0>
		{ };

		template<typename _Tp, unsigned _Uint, size_t _Size>
		struct extent<_Tp[_Size], _Uint>
				: public IntegralConstant<size_t, _Uint == 0 ? _Size : extent<_Tp, _Uint - 1>::value>
		{ };

		template<typename _Tp, unsigned _Uint>
		struct extent<_Tp[], _Uint> : public IntegralConstant<size_t, _Uint == 0 ? 0 : extent<_Tp, _Uint - 1>::value>
		{ };

		template<typename _Tp>
		struct __is_array_known_bounds
				: public IntegralConstant<bool, (extent<_Tp>::value > 0)>
		{ };

		template<typename _Tp>
		struct __is_array_unknown_bounds : public And<IsArray<_Tp>, Not<extent<_Tp>>>
		{ };

		struct __do_is_destructible_impl
		{
			template<typename _Tp, typename = decltype(declval<_Tp&>().~_Tp())>
			static TrueType __test(int)
			{ return {}; }

			template<typename>
			static FalseType __test(...)
			{ return {}; }
		};

		template<typename _Tp>
		struct __is_destructible_impl
				: public __do_is_destructible_impl
		{
			typedef decltype(__test<_Tp>(0)) type;
		};

		template<typename _Tp, bool = Or<IsSame<_Tp, void>,
				__is_array_unknown_bounds<_Tp>,
				IsFunction<_Tp>>::value, bool = Or<IsReference<_Tp>, IsScalar<_Tp>>::value>
		struct __is_destructible_safe;

		template<typename _Tp>
		struct __is_destructible_safe<_Tp, false, false>
				: public __is_destructible_impl<typename RemoveAllExtents<_Tp>::type>::type
		{ };

		template<typename _Tp>
		struct __is_destructible_safe<_Tp, true, false>
				: public FalseType { };

		template<typename _Tp>
		struct __is_destructible_safe<_Tp, false, true>
				: public TrueType { };

		template<typename _Tp>
		struct IsDestructible : public __is_destructible_safe<_Tp>::type
		{ };
	}
}
