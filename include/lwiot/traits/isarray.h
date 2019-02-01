/*
 * IsArray type trait.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/types.h>
#include <lwiot/traits/integralconstant.h>

namespace lwiot
{
	namespace traits
	{
		template <typename>
		struct IsArray : public FalseType
		{ };

		template <typename T, size_t size>
		struct IsArray<T[size]> : TrueType
		{ };

		template <typename T>
		struct IsArray<T[]> : TrueType
		{ };
	}
}
