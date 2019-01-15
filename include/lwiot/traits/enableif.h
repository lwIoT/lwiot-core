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
	struct enable_if { };

	template <typename T>
	struct enable_if<true, T> {
		typedef T type;
	};
}}
