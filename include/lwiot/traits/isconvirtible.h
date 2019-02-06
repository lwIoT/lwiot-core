/*
 * IsConvirtible definition & implementation.
 *
 * @author Michel Megens
 * @email  dev@biejte.net
 */

#pragma once

#include <lwiot/traits/integralconstant.h>
#include <lwiot/traits/isfunction.h>
#include <lwiot/traits/issame.h>

namespace lwiot
{
	namespace traits
	{
		template <typename T>
		using IsVoid = IsSame<T, void>;

		template<typename _From, typename _To,
				bool = Or<IsVoid<_From>, IsFunction<_To>,
		IsArray<_To>>::value>
		struct __is_convertible_helper
		{ typedef typename IsVoid <_To>::type type; };

		template<typename _From, typename _To>
		class __is_convertible_helper<_From, _To, false>
		{
			template<typename _To1>
			static void __test_aux(_To1) { }

			template<typename _From1, typename _To1,
					typename = decltype(__test_aux<_To1>(traits::declval<_From1>()))>
			static TrueType __test(int)
			{
				return TrueType();
			}

			template<typename, typename>
			static FalseType __test(...) { return FalseType(); }

		public:
			typedef decltype(__test<_From, _To>(0)) type;
		};

		template<typename _From, typename _To>
		struct IsConvirtable : public __is_convertible_helper<_From, _To>::type
		{ };
	}
}
