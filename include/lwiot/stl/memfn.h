/*
 * MemFn implementation.
 *
 * @author Michel Megens
 * @email  michel@michelmegens.net
 */

#pragma once

#include <lwiot/types.h>

#include <lwiot/traits/ismember.h>
#include <lwiot/traits/isreference.h>
#include <lwiot/traits/isconvirtible.h>
#include <lwiot/traits/decay.h>

#include <lwiot/stl/referencewrapper.h>
#include <lwiot/stl/move.h>
#include <lwiot/stl/forward.h>

#define _LWIOT_MEM_FN_TRAITS2(_CV, _REF, _LVAL, _RVAL)        \
  template<typename _Res, typename _Class, typename... _ArgTypes>    \
    struct _Mem_fn_traits<_Res (_Class::*)(_ArgTypes...) _CV _REF>    \
    : _Mem_fn_traits_base<_Res, _CV _Class, _ArgTypes...>        \
    {                                    \
      using __pmf_type  = _Res (_Class::*)(_ArgTypes...) _CV _REF;    \
      using __lvalue = _LVAL;                        \
      using __rvalue = _RVAL;                        \
      using __vararg = lwiot::traits::FalseType;                    \
    };                                    \
  template<typename _Res, typename _Class, typename... _ArgTypes>    \
    struct _Mem_fn_traits<_Res (_Class::*)(_ArgTypes..., ...) _CV _REF>    \
    : _Mem_fn_traits_base<_Res, _CV _Class, _ArgTypes...>        \
    {                                    \
      using __pmf_type  = _Res (_Class::*)(_ArgTypes..., ...) _CV _REF;    \
      using __lvalue = _LVAL;                        \
      using __rvalue = _RVAL;                        \
      using __vararg = lwiot::traits::TrueType;                    \
    };

#define _LWIOT_MEM_FN_TRAITS(_REF, _LVAL, _RVAL)        \
  _LWIOT_MEM_FN_TRAITS2(        , _REF, _LVAL, _RVAL)    \
  _LWIOT_MEM_FN_TRAITS2(const        , _REF, _LVAL, _RVAL)    \
  _LWIOT_MEM_FN_TRAITS2(volatile    , _REF, _LVAL, _RVAL)    \
  _LWIOT_MEM_FN_TRAITS2(const volatile, _REF, _LVAL, _RVAL)

namespace lwiot
{
	namespace stl
	{
		template<typename _Arg, typename _Result>
		struct UnaryFunction {
			typedef _Arg argument_type;
			typedef _Result result_type;
		};

		template<typename _Arg1, typename _Arg2, typename _Result>
		struct BinaryFunction {
			typedef _Arg1 first_argument_type;
			typedef _Arg2 second_argument_type;
			typedef _Result result_type;
		};

		namespace detail
		{
			template<typename... _Types>
			struct _Pack : traits::IntegralConstant<size_t, sizeof...(_Types)> {
			};

			template<typename _From, typename _To, bool = _From::value == _To::value>
			struct _AllConvertible : traits::FalseType {
			};

			template<typename... _From, typename... _To>
			struct _AllConvertible<_Pack<_From...>, _Pack<_To...>, true>
					: traits::And<traits::IsConvirtable<_From, _To>...> {
			};


			template<typename _Res, typename... _ArgTypes>
			struct _Maybe_unary_or_binary_function {
			};

			template<typename _Res, typename _T1>
			struct _Maybe_unary_or_binary_function<_Res, _T1> : UnaryFunction<_T1, _Res> {
			};

			template<typename _Res, typename _T1, typename _T2>
			struct _Maybe_unary_or_binary_function<_Res, _T1, _T2> : BinaryFunction<_T1, _T2, _Res> {
			};

			template<typename _Signature>
			struct _Mem_fn_traits;

			template<typename _Res, typename _Class, typename... _ArgTypes>
			struct _Mem_fn_traits_base {
				using __result_type = _Res;
				using __class_type =  _Class;
				using __arg_types = _Pack<_ArgTypes...>;
				using __maybe_type = _Maybe_unary_or_binary_function<_Res, _Class *, _ArgTypes...>;
				using __arity = traits::IntegralConstant<size_t, sizeof...(_ArgTypes)>;
			};

			_LWIOT_MEM_FN_TRAITS(, traits::TrueType, traits::TrueType)
			_LWIOT_MEM_FN_TRAITS(&, traits::TrueType, traits::FalseType)
			_LWIOT_MEM_FN_TRAITS(&&, traits::FalseType, traits::TrueType)

			template<typename... _Cond>
			using _Require = typename traits::EnableIf<traits::And<_Cond...>::value>::type;

			template<typename _Tp1, typename _Tp2>
			using NotSame = traits::Not<traits::IsSame<typename traits::Decay<_Tp1>::type,
					typename traits::Decay<_Tp2>::type>>;
		}

		template<typename _MemFunPtr, bool __is_mem_fn = traits::IsMemberFunctionPointer<_MemFunPtr>::value>
		class _Mem_fn_base : public detail::_Mem_fn_traits<_MemFunPtr>::__maybe_type {
			using _Traits = detail::_Mem_fn_traits<_MemFunPtr>;

			using _Class = typename _Traits::__class_type;
			using _ArgTypes = typename _Traits::__arg_types;
			using _Pmf = typename _Traits::__pmf_type;


			template<typename _Func, typename... _BoundArgs>
			friend
			struct _Bind_check_arity;

			// for varargs functions we just check the number of arguments,
			// otherwise we also check they are convertible.
		public:
			using _Arity = typename _Traits::__arity;
			using _Varargs = typename _Traits::__vararg;
			template<typename _Args>
			using _CheckArgs = typename traits::Conditional<_Varargs::value,
					traits::IntegralConstant<bool, (_Args::value >= _ArgTypes::value)>,
					detail::_AllConvertible<_Args, _ArgTypes>>::type;

			using result_type = typename _Traits::__result_type;

			explicit _Mem_fn_base(_Pmf __pmf) : _M_pmf(__pmf)
			{
			}

			// Handle objects
			template<typename... _Args, typename _Req = detail::_Require<typename _Traits::__lvalue,
					_CheckArgs<detail::_Pack < _Args...>>>> result_type

			operator()(_Class &__object, _Args &&... __args) const
			{
				return (__object.*_M_pmf)(stl::forward<_Args>(__args)...);
			}

			template<typename... _Args, typename _Req = detail::_Require<typename _Traits::__rvalue,
					_CheckArgs<detail::_Pack < _Args...>>>>
			result_type

			operator()(_Class &&__object, _Args &&... __args) const
			{
				return (stl::move(__object).*_M_pmf)(stl::forward<_Args>(__args)...);
			}

			// Handle pointers
			template<typename... _Args, typename _Req = detail::_Require<typename _Traits::__lvalue, _CheckArgs<detail::_Pack < _Args...>>>> result_type

			operator()(_Class *__object, _Args &&... __args) const
			{
				return (__object->*_M_pmf)(stl::forward<_Args>(__args)...);
			}

			// Handle smart pointers, references and pointers to derived
			template<typename _Tp, typename... _Args, typename _Req
			= detail::_Require <detail::NotSame<_Class, _Tp>, detail::NotSame<_Class *, _Tp>,
			_CheckArgs<detail::_Pack < _Args...>>>>
			result_type

			operator()(_Tp &&__object, _Args &&... __args) const
			{
				return _M_call(stl::forward<_Tp>(__object), &__object,
				               stl::forward<_Args>(__args)...);
			}

			// Handle reference wrappers
			template<typename _Tp, typename... _Args, typename _Req
			= detail::_Require<traits::IsBaseOf<_Class, _Tp>, typename _Traits::__lvalue, _CheckArgs<detail::_Pack < _Args...>>>> result_type

			operator()(stl::ReferenceWrapper<_Tp> __ref, _Args &&... __args) const
			{
				return operator()(__ref.get(), stl::forward<_Args>(__args)...);
			}

		private:
			template<typename _Tp, typename... _Args>
			result_type _M_call(_Tp &&__object, const volatile _Class *, _Args &&... __args) const
			{
				return (stl::forward<_Tp>(__object).*_M_pmf) (stl::forward<_Args>(__args)...);
			}

			template<typename _Tp, typename... _Args>
			result_type _M_call(_Tp &&__ptr, const volatile void *, _Args &&... __args) const
			{
				return ((*__ptr).*_M_pmf)(stl::forward<_Args>(__args)...);
			}

			_Pmf _M_pmf;
		};

		// Partial specialization for member object pointers.
		template<typename _Res, typename _Class>
		class _Mem_fn_base<_Res _Class::*, false> {
			using __pm_type = _Res _Class::*;

			// This bit of genius is due to Peter Dimov, improved slightly by
			// Douglas Gregor.
			// Made less elegant to support perfect forwarding and noexcept.
			template<typename _Tp>
			auto
			_M_call(_Tp &&__object, const _Class *) const noexcept
			-> decltype(stl::forward<_Tp>(__object).*traits::declval<__pm_type &>())
			{
				return stl::forward<_Tp>(__object).*_M_pm;
			}

			template<typename _Tp, typename _Up>
			auto
			_M_call(_Tp &&__object, _Up *const *) const noexcept
			-> decltype((*stl::forward<_Tp>(__object)).*traits::declval<__pm_type &>())
			{
				return (*stl::forward<_Tp>(__object)).*_M_pm;
			}

			template<typename _Tp>
			auto
			_M_call(_Tp &&__ptr, const volatile void *) const
			noexcept(noexcept((*__ptr).*traits::declval<__pm_type &>()))
			-> decltype((*__ptr).*traits::declval<__pm_type &>())
			{
				return (*__ptr).*_M_pm;
			}


			template<typename _Func, typename... _BoundArgs>
			friend struct _Bind_check_arity;

		public:
			using _Arity = traits::IntegralConstant<size_t, 0>;
			using _Varargs = traits::FalseType;

			explicit
			_Mem_fn_base(_Res _Class::*__pm) noexcept : _M_pm(__pm)
			{
			}

			// Handle objects
			_Res &
			operator()(_Class &__object) const noexcept
			{
				return __object.*_M_pm;
			}

			const _Res &
			operator()(const _Class &__object) const noexcept
			{
				return __object.*_M_pm;
			}

			_Res &&
			operator()(_Class &&__object) const noexcept
			{
				return stl::forward<_Class>(__object).*_M_pm;
			}

			const _Res &&
			operator()(const _Class &&__object) const noexcept
			{
				return stl::forward<const _Class>(__object).*_M_pm;
			}

			// Handle pointers
			_Res &
			operator()(_Class *__object) const noexcept
			{
				return __object->*_M_pm;
			}

			const _Res &
			operator()(const _Class *__object) const noexcept
			{
				return __object->*_M_pm;
			}

			// Handle smart pointers and derived
			template<typename _Tp, typename _Req = detail::_Require <detail::NotSame<_Class *, _Tp>>>
			auto
			operator()(_Tp &&__unknown) const
			noexcept(noexcept(traits::declval<_Mem_fn_base *>()->_M_call
					(stl::forward<_Tp>(__unknown), &__unknown)))
			-> decltype(this->_M_call(stl::forward<_Tp>(__unknown), &__unknown))
			{
				return _M_call(stl::forward<_Tp>(__unknown), &__unknown);
			}

			template<typename _Tp, typename _Req = detail::_Require <traits::IsBaseOf<_Class, _Tp>>>
			auto operator()(stl::ReferenceWrapper <_Tp> __ref) const
			noexcept(noexcept(traits::declval<_Mem_fn_base &>()(__ref.get())))
			-> decltype((*this)(__ref.get()))
			{
				return (*this)(__ref.get());
			}

		private:
			_Res _Class::*_M_pm;
		};

		template <typename>
		struct _Mem_fn;

		template<typename _Res, typename _Class>
		struct _Mem_fn<_Res _Class::*>
				: _Mem_fn_base<_Res _Class::*> {
			using _Mem_fn_base<_Res _Class::*>::_Mem_fn_base;
		};

		/**
		 *  @brief Returns a function object that forwards to the member pointer __pm;
		 *  @param __pm Member function.
		 *  @return Function object towards \p __pm.
		 */
		template<typename _Tp, typename _Class>
		inline _Mem_fn<_Tp _Class::*>
		mem_fn(_Tp _Class::* __pm) noexcept
		{
			return _Mem_fn<_Tp _Class::*>(__pm);
		}
	}
}
