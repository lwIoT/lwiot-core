/*
 * Is constructible type traits.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/traits/integralconstant.h>
#include <lwiot/traits/isreference.h>
#include <lwiot/traits/isdestructible.h>
#include <lwiot/traits/isarray.h>

namespace lwiot
{
	namespace traits
	{
		struct __do_is_direct_constructible_impl
		{
			template<typename _Tp, typename _Arg, typename = decltype(::new _Tp(declval<_Arg>()))>
			static TrueType __test(int)
			{ return { }; }

			template<typename, typename>
			static FalseType __test(...)
			{ return { }; }
		};

		template<typename _Tp, typename _Arg>
		struct __is_direct_constructible_impl : public __do_is_direct_constructible_impl
		{
			typedef decltype(__test<_Tp, _Arg>(0)) type;
		};

		template<typename _Tp, typename _Arg>
		struct __is_direct_constructible_new_safe : public And<IsDestructible<_Tp>, __is_direct_constructible_impl<_Tp, _Arg>>
		{ };

		template<typename _From, typename _To, bool = And<IsLValueReference<_From>, IsRValueReference<_To>>::value>
		struct __is_lvalue_to_rvalue_ref;

		// Detect whether we have an lvalue of non-function type
		// bound to a reference-compatible rvalue-reference.
		template<typename _From, typename _To>
		struct __is_lvalue_to_rvalue_ref<_From, _To, true>
		{
			typedef typename RemoveCv<typename RemoveReference< _From>::type>::type __src_t;
			typedef typename RemoveCv<typename RemoveReference< _To>::type>::type __dst_t;
			typedef And<Not<IsFunction<__src_t>>, Or<IsSame<__src_t, __dst_t>, IsBaseOf<__dst_t, __src_t>>> type;
			static constexpr bool value = type::value;
		};

		template<typename _From, typename _To>
		struct __is_lvalue_to_rvalue_ref<_From, _To, false> : public FalseType
		{ };

		struct __do_is_static_castable_impl
		{
			template<typename _From, typename _To, typename = decltype(static_cast<_To>(declval<_From>()))>
			static TrueType __test(int)
			{ return { }; }

			template<typename, typename>
			static FalseType __test(...)
			{ return { }; }
		};

		template<typename _From, typename _To>
		struct __is_static_castable_impl
				: public __do_is_static_castable_impl
		{
			typedef decltype(__test<_From, _To>(0)) type;
		};

		template<typename _From, typename _To>
		struct __is_static_castable_safe
				: public __is_static_castable_impl<_From, _To>::type
		{ };

		// __is_static_castable
		template<typename _From, typename _To>
		struct __is_static_castable
				: public IntegralConstant<bool, (__is_static_castable_safe< _From, _To>::value)>
		{ };

		template <typename , typename ...>
		struct IsConstructible;

		template<typename _Base, typename _Derived>
		struct is_base_of
				: public IntegralConstant<bool, __is_base_of(_Base, _Derived)>
		{ };

		template<typename _From, typename , bool = Not<Or<IsSame<_From, void>, IsFunction<_From>>>::value>
		struct __is_base_to_derived_ref;

		template<typename _From, typename _To>
		struct __is_base_to_derived_ref<_From, _To, true>
		{
			typedef typename RemoveCv<typename RemoveReference<_From >::type>::type __src_t;
			typedef typename RemoveCv<typename RemoveReference<_To >::type>::type __dst_t;
			typedef And<Not<IsSame<__src_t, __dst_t>>, is_base_of<__src_t, __dst_t>, Not<IsConstructible<__dst_t, _From>>> type;
			static constexpr bool value = type::value;
		};

		template<typename _From, typename _To>
		struct __is_base_to_derived_ref<_From, _To, false> : public FalseType
		{ };

		template<typename _Tp, typename _Arg>
		struct __is_direct_constructible_ref_cast
				: public And<__is_static_castable < _Arg, _Tp>,
				  Not<Or < __is_base_to_derived_ref < _Arg, _Tp>,
				  __is_lvalue_to_rvalue_ref<_Arg, _Tp> >>> {
		};

		template<typename _Tp, typename _Arg>
		struct __is_direct_constructible_new
				: public Conditional<IsReference<_Tp>::value,
						__is_direct_constructible_ref_cast < _Tp, _Arg>,
				  __is_direct_constructible_new_safe<_Tp, _Arg> >::type {
		};

		template<typename _Tp, typename _Arg>
		struct __is_direct_constructible
				: public __is_direct_constructible_new<_Tp, _Arg>::type {
		};

		struct __do_is_nary_constructible_impl {
			template<typename _Tp, typename... _Args, typename = decltype(_Tp(declval<_Args>()...))>
			static TrueType __test(int)
			{
				return {};
			}

			template<typename, typename...>
			static FalseType __test(...)
			{
				return {};
			}
		};

		template<typename _Tp, typename... _Args>
		struct __is_nary_constructible_impl
				: public __do_is_nary_constructible_impl {
			typedef decltype(__test<_Tp, _Args...>(0)) type;
		};

		template<typename _Tp, typename... _Args>
		struct __is_nary_constructible
				: public __is_nary_constructible_impl<_Tp, _Args...>::type {
			static_assert(sizeof...(_Args) > 1,
						  "Only useful for > 1 arguments");
		};


		struct __do_is_default_constructible_impl
		{
			template<typename _Tp, typename = decltype(_Tp())>
			static TrueType __test(int)
			{
				return {};
			}

			template<typename>
			static FalseType __test(...)
			{
				return {};
			}
		};

		template<typename _Tp>
		struct __is_default_constructible_impl : public __do_is_default_constructible_impl
		{
			typedef decltype(__test<_Tp>(0)) type;
		};

		template<typename _Tp>
		struct __is_default_constructible_atom :
				public And<Not<IsSame<_Tp, void>>, __is_default_constructible_impl<_Tp>>
		{ };

		template<typename _Tp, bool = IsArray<_Tp>::value>
		struct __is_default_constructible_safe;

		template<typename _Tp>
		struct __is_default_constructible_safe<_Tp, true>
				: public And<__is_array_known_bounds<_Tp>, __is_default_constructible_atom<typename RemoveAllExtents<_Tp>::type>>
		{ };

		template<typename _Tp>
		struct __is_default_constructible_safe<_Tp, false>
				: public __is_default_constructible_atom<_Tp>::type
		{ };

		/// is_default_constructible
		template<typename _Tp>
		struct IsDefaultConstructible
				: public __is_default_constructible_safe<_Tp>::type
		{ };

		template<typename _Tp, typename... _Args>
		struct __is_constructible_impl
				: public __is_nary_constructible<_Tp, _Args...> {
		};

		template<typename _Tp, typename _Arg>
		struct __is_constructible_impl<_Tp, _Arg>
				: public __is_direct_constructible<_Tp, _Arg> {
		};

		template<typename _Tp>
		struct __is_constructible_impl<_Tp> : public IsDefaultConstructible<_Tp> {
		};

		template<typename _Tp, typename... _Args>
		struct IsConstructible
				: public __is_constructible_impl<_Tp, _Args...>::type {
		};
	}
}