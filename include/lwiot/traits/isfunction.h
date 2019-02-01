/*
 * IsFunction type trait.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/traits/integralconstant.h>

#ifdef WIN32
#include <type_traits>
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
#ifdef WIN32
		template <typename T>
		using IsFunction = std::is_function<T>;
#else
		template<typename>
		struct IsFunction : public lwiot::traits::FalseType {
		};

		template<typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
		struct IsFunction<_Res(_ArgTypes...) _GLIBCXX_NOEXCEPT_QUAL>
				: public TrueType {
		};

		template<typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
		struct IsFunction<_Res(_ArgTypes...) & _GLIBCXX_NOEXCEPT_QUAL>
				: public TrueType {
		};

		template<typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
		struct IsFunction<_Res(_ArgTypes...) && _GLIBCXX_NOEXCEPT_QUAL>
				: public TrueType {
		};

		template<typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
		struct IsFunction<_Res(_ArgTypes......) _GLIBCXX_NOEXCEPT_QUAL>
				: public TrueType {
		};

		template<typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
		struct IsFunction<_Res(_ArgTypes......) & _GLIBCXX_NOEXCEPT_QUAL>
				: public TrueType {
		};

		template<typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
		struct IsFunction<_Res(_ArgTypes......) && _GLIBCXX_NOEXCEPT_QUAL>
				: public TrueType {
		};

		template<typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
		struct IsFunction<_Res(_ArgTypes...) const _GLIBCXX_NOEXCEPT_QUAL>
				: public TrueType {
		};

		template<typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
		struct IsFunction<_Res(_ArgTypes...) const & _GLIBCXX_NOEXCEPT_QUAL>
				: public TrueType {
		};

		template<typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
		struct IsFunction<_Res(_ArgTypes...) const && _GLIBCXX_NOEXCEPT_QUAL>
				: public TrueType {
		};

		template<typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
		struct IsFunction<_Res(_ArgTypes......) const _GLIBCXX_NOEXCEPT_QUAL>
				: public TrueType {
		};

		template<typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
		struct IsFunction<_Res(_ArgTypes......) const & _GLIBCXX_NOEXCEPT_QUAL>
				: public TrueType {
		};

		template<typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
		struct IsFunction<_Res(_ArgTypes......) const && _GLIBCXX_NOEXCEPT_QUAL>
				: public TrueType {
		};

		template<typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
		struct IsFunction<_Res(_ArgTypes...) volatile _GLIBCXX_NOEXCEPT_QUAL>
				: public TrueType {
		};

		template<typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
		struct IsFunction<_Res(_ArgTypes...) volatile & _GLIBCXX_NOEXCEPT_QUAL>
				: public TrueType {
		};

		template<typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
		struct IsFunction<_Res(_ArgTypes...) volatile && _GLIBCXX_NOEXCEPT_QUAL>
				: public TrueType {
		};

		template<typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
		struct IsFunction<_Res(_ArgTypes......) volatile _GLIBCXX_NOEXCEPT_QUAL>
				: public TrueType {
		};

		template<typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
		struct IsFunction<_Res(_ArgTypes......) volatile & _GLIBCXX_NOEXCEPT_QUAL>
				: public TrueType {
		};

		template<typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
		struct IsFunction<_Res(_ArgTypes......) volatile && _GLIBCXX_NOEXCEPT_QUAL>
				: public TrueType {
		};

		template<typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
		struct IsFunction<_Res(_ArgTypes...) const volatile _GLIBCXX_NOEXCEPT_QUAL>
				: public TrueType {
		};

		template<typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
		struct IsFunction<_Res(_ArgTypes...) const volatile & _GLIBCXX_NOEXCEPT_QUAL>
				: public TrueType {
		};

		template<typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
		struct IsFunction<_Res(_ArgTypes...) const volatile && _GLIBCXX_NOEXCEPT_QUAL>
				: public TrueType {
		};

		template<typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
		struct IsFunction<_Res(_ArgTypes......) const volatile _GLIBCXX_NOEXCEPT_QUAL>
				: public TrueType {
		};

		template<typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
		struct IsFunction<_Res(_ArgTypes......) const volatile & _GLIBCXX_NOEXCEPT_QUAL>
				: public TrueType {
		};

		template<typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
		struct IsFunction<_Res(_ArgTypes......) const volatile && _GLIBCXX_NOEXCEPT_QUAL>
				: public TrueType {
		};
#endif /* WIN32 */
	}
}
