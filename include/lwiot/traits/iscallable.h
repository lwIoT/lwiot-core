/*
 * IsCallable type trait.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/traits/integralconstant.h>

namespace lwiot
{
	namespace traits
	{

		namespace detail
		{
			template<typename _Tp, typename _Up = _Tp&&>
			_Up
			__declval(int);

			template<typename _Tp>
			_Tp
			__declval(long);

			template<typename _Tp>
			auto declval() noexcept -> decltype(__declval<_Tp>(0));
		}

		template <typename T, typename... Args>
		struct IsCallable {
			template<class U>
			static auto test(U*p) -> decltype((*p)(detail::declval<Args>()...), void(), TrueType())
			{
				return TrueType();
			}

			template <typename U>
			static auto test(...) -> decltype(FalseType())
			{
				return FalseType();
			}

			static constexpr auto value = decltype(test<T>(nullptr))::value;
		};
	}
}
