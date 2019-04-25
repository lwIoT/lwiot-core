/*
 * RemoveExtent type trait.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/types.h>

namespace lwiot
{
	namespace traits
	{
		template <typename T>
		struct RemoveExtent {
			typedef T type;
		};

		template <typename T, size_t size>
		struct RemoveExtent<T[size]> {
			typedef T type;
		};

		template <typename T>
		struct RemoveExtent<T[]> {
			typedef T type;
		};
	}
}
