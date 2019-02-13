/*
 * Enable if type trait.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

namespace lwiot { namespace traits
{
	template <bool, typename T = void>
	struct EnableIf { };

	template <typename T>
	struct EnableIf<true, T> {
		typedef T type;
	};

	template <bool condition, typename T = void>
	using EnableIf_t = typename EnableIf<condition, T>::type;
}}
