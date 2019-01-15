/*
 * Type trait: IntegralConstant.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/traits/integralconstant.h>

namespace lwiot { namespace traits {
	/* Check for lvalue references */
	template <typename>
	struct IsLValueReference : public FalseType
	{ };

	template <typename T>
	struct IsLValueReference<T&> : public TrueType
	{ };

	template <typename>
	struct IsRValueReference : public FalseType
	{ };

	template <typename T>
	struct IsRValueReference<T&&> : public TrueType
	{ };

	template <typename T>
	struct IsReference : public Or<IsLValueReference<T>, IsRValueReference<T>>::type
	{ };
}}
