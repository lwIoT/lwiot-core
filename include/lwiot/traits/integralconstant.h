/*
 * Type trait: IntegralConstant.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

namespace lwiot { namespace traits {
		template <typename T, T _V>
		struct IntegralConstant {
			static constexpr T value = _V;
			typedef T value_type;
			typedef IntegralConstant<T, _V> type;
			explicit constexpr operator value_type() const noexcept { return value; }
			constexpr value_type operator()() const noexcept { return value; }
		};

		typedef IntegralConstant<bool, true> TrueType;
		typedef IntegralConstant<bool, false> FalseType;

		/* Conditional */
		template<bool _Cond, typename _Iftrue, typename _Iffalse>
		struct Conditional {
			typedef _Iftrue type;
		};

		/* Partial specialization for false */
		template<typename _Iftrue, typename _Iffalse>
		struct Conditional<false, _Iftrue, _Iffalse> {
			typedef _Iffalse type;
		};

		/* OR and AND */
		template<typename...>
		struct Or;

		template<>
		struct Or<> : public FalseType
		{ };

		template<typename _B1>
		struct Or<_B1> : public _B1
		{ };

		template<typename _B1, typename _B2>
		struct Or<_B1, _B2> : public Conditional<_B1::value, _B1, _B2>::type
		{ };

		template<typename _B1, typename _B2, typename _B3, typename... _Bn>
		struct Or<_B1, _B2, _B3, _Bn...> : public Conditional<_B1::value, _B1, Or<_B2, _B3, _Bn...>>::type
		{ };

		template<typename...>
		struct And;

		template<>
		struct And<> : public TrueType
		{ };

		template<typename _B1>
		struct And<_B1> : public _B1
		{ };

		template<typename _B1, typename _B2>
		struct And<_B1, _B2> : public Conditional<_B1::value, _B2, _B1>::type
		{ };

		template<typename _B1, typename _B2, typename _B3, typename... _Bn>
		struct And<_B1, _B2, _B3, _Bn...> :
				public Conditional<_B1::value, And<_B2, _B3, _Bn...>, _B1>::type
		{ };

		template <bool value>
		using BoolConstant = IntegralConstant<bool, value>;

		template<typename _Pp>
		struct Not : public BoolConstant <!bool(_Pp::value)>
		{ };
}}
