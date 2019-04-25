/*
 * Type trait to compare two types.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/traits/integralconstant.h>

namespace lwiot { namespace traits
{
	template <typename T, typename V>
	struct IsSame : FalseType { };

	template <typename T>
	struct IsSame<T,T> : TrueType { };
}}
