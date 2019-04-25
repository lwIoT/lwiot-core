/*
 * Type trait: IntegralConstant.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/traits/integralconstant.h>
#include <lwiot/traits/issame.h>
#include <lwiot/traits/isfunction.h>

#ifdef WIN32
#endif

#ifndef _GLIBCXX_NOEXCEPT_PARM
#define _GLIBCXX_NOEXCEPT_PARM
#endif

#ifndef _GLIBCXX_NOEXCEPT_QUAL
#define _GLIBCXX_NOEXCEPT_QUAL
#endif

namespace lwiot
{
	namespace traits
	{
		/* Check for lvalue references */
		template<typename>
		struct IsLValueReference : public FalseType {
		};

		template<typename T>
		struct IsLValueReference<T &> : public TrueType {
		};

		template<typename>
		struct IsRValueReference : public FalseType {
		};

		template<typename T>
		struct IsRValueReference<T &&> : public TrueType {
		};

		template<typename T>
		struct IsReference : public Or<IsLValueReference<T>, IsRValueReference<T>>::type {
		};

		template <typename>
		struct IsObject;

		/* IsReferenceable */
		template<typename _Tp>
		struct IsReferenceable : public traits::Or<IsObject < _Tp>, IsReference<_Tp>> ::type
		{
		};

#ifndef WIN32
		template<typename _Res, typename ... _Args
				_GLIBCXX_NOEXCEPT_PARM>
		struct IsReferenceable<_Res(_Args...) _GLIBCXX_NOEXCEPT_QUAL>
				: public traits::TrueType {
		};

		template<typename _Res, typename ... _Args
				_GLIBCXX_NOEXCEPT_PARM>
		struct IsReferenceable<_Res(_Args......) _GLIBCXX_NOEXCEPT_QUAL> : public TrueType {
		};
#endif

		template<typename T>
		struct IsObject : public Not<Or <IsFunction<T>, IsReference<T>, IsSame<T,void>>>::type
		{
		};

		template<typename _Tp, bool = IsReferenceable<_Tp>::value>
		struct __add_rvalue_reference_helper
		{ typedef _Tp   type; };

		template<typename _Tp>
		struct __add_rvalue_reference_helper<_Tp, true>
		{ typedef _Tp&&   type; };

		/// add_rvalue_reference
		template<typename _Tp>
		struct add_rvalue_reference
				: public __add_rvalue_reference_helper<_Tp>
		{ };

		template<typename _Tp>
		struct __declval_protector
		{
			static const bool __stop = false;
			static typename add_rvalue_reference<_Tp>::type __delegate() { }
		};

		template<typename _Tp>
		inline typename add_rvalue_reference<_Tp>::type
		declval() noexcept
		{
			static_assert(__declval_protector<_Tp>::__stop,
			              "declval() must not be used!");
			return __declval_protector<_Tp>::__delegate();
		}
}
}
