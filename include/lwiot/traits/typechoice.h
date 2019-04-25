/*
 * Boolean type choice. Enables type definition at
 * compile time.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

namespace lwiot { namespace traits
{
	template <bool flag, typename true_type, typename false_type>
	struct TypeChoice;

	template <typename true_type, typename false_type>
	struct TypeChoice<true, true_type, false_type> {
		typedef true_type type;
	};

	template <typename true_type, typename false_type>
	struct TypeChoice<false, true_type, false_type> {
		typedef false_type type;
	};
}}
