/*
 * Enable if type trait.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

namespace lwiot { namespace traits
{
	template <bool, typename T>
	struct EnableIf { };

	template <typename T>
	struct EnableIf<true, T> {
		typedef T type;
	};
}}
