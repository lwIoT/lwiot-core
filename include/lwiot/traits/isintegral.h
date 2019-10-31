/*
 * Integral check type trait.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/traits/integralconstant.h>

namespace lwiot { namespace traits
{
	template <typename>
	struct IsIntegral_helper : FalseType { };

	template <>
	struct IsIntegral_helper<bool> : TrueType { };

	template <>
	struct IsIntegral_helper<char16_t> : TrueType { };

	template <>
	struct IsIntegral_helper<char32_t> : TrueType { };

	/* Signed types */
	template <>
	struct IsIntegral_helper<char> : TrueType { };
	template <>
	struct IsIntegral_helper<short> : TrueType { };
	template <>
	struct IsIntegral_helper<int> : TrueType { };
	template <>
	struct IsIntegral_helper<long> : TrueType { };
	template <>
	struct IsIntegral_helper<long long> : TrueType { };

	/* Unsigned types */
	template <>
	struct IsIntegral_helper<unsigned char> : TrueType { };
	template <>
	struct IsIntegral_helper<unsigned short> : TrueType { };
	template <>
	struct IsIntegral_helper<unsigned int> : TrueType { };
	template <>
	struct IsIntegral_helper<unsigned long> : TrueType { };
	template <>
	struct IsIntegral_helper<unsigned long long> : TrueType { };

	template <typename T>
	struct IsIntegral : IsIntegral_helper<T> {
		static constexpr bool Value = IsIntegral_helper<T>::value;
	};
}}
