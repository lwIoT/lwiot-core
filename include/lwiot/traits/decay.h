/*
 * Decay type trait definition & implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/traits/removecv.h>
#include <lwiot/traits/remove_extent.h>
#include <lwiot/traits/removereference.h>
#include <lwiot/traits/isarray.h>
#include <lwiot/traits/addpointer.h>

namespace lwiot
{
	namespace traits
	{
		template <typename T, bool is_ary = IsArray<T>::value, bool is_func = IsFunction<T>::value>
		struct DecaySelector;

		template <typename T>
		struct DecaySelector<T, false, false>
		{
			typedef typename RemoveReference<T>::type __type;
		};

		template <typename T>
		struct DecaySelector<T, true, false>
		{
			typedef typename RemoveExtent<T>::type* __type;
		};

		template <typename T>
		struct DecaySelector<T, false, true>
		{
			typedef typename RemoveExtent<T>::type* __type;
		};

		template <typename T>
		struct Decay {
		private:
			typedef typename RemoveReference<T>::type  __remove_ref;

		public:
			typedef typename DecaySelector<__remove_ref>::__type type;
		};
	}
}
