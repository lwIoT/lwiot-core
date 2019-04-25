/*
 * Floating point type trait.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/traits/removecv.h>

namespace lwiot { namespace traits
{
	template <typename>
	struct IsFloatingPoint_helper : FalseType { };

	template <>
	struct IsFloatingPoint_helper<float> : TrueType { };

	template <>
	struct IsFloatingPoint_helper<double> : TrueType { };

	template <>
	struct IsFloatingPoint_helper<long double> : TrueType { };

	template <typename T>
	struct IsFloatingPoint : public IsFloatingPoint_helper<typename RemoveCv<T>::type>::type
	{ };
}}
