/*
 * Bind header/implementation.
 *
 * @author Michel Megens
 * @email  michel@michelmegens.net
 */

/**
 * @file bind.h Bind header.
 */

/// @addtogroup stl
/// @{

#pragma once

#include <lwiot/types.h>

#include <lwiot/stl/move.h>
#include <lwiot/stl/forward.h>
#include <lwiot/stl/referencewrapper.h>
#include <lwiot/stl/integersequence.h>
#include <lwiot/stl/tuple.h>

#include <lwiot/traits/decay.h>
#include <lwiot/traits/isscalar.h>
#include <lwiot/traits/isreference.h>

#include <lwiot/traits/decay.h>
#include <lwiot/traits/removecv.h>
#include <lwiot/traits/enableif.h>

#include <lwiot/stl/memfn.h>

namespace lwiot
{
	namespace stl
	{
		/* weak result type */
		namespace detail
		{
			template<typename _Functor, typename = __void_t<>>
			struct _Maybe_get_result_type {
			};

			template<typename _Functor>
			struct _Maybe_get_result_type<_Functor, __void_t < typename _Functor::result_type>> {
				typedef typename _Functor::result_type result_type;
			};

			template<typename _Functor>
			struct _Weak_result_type_impl : _Maybe_get_result_type<_Functor> {
			};

			// Retrieve the result type for a function type.
			template<typename _Res, typename... _ArgTypes>
			struct _Weak_result_type_impl<_Res(_ArgTypes...)> {
				typedef _Res result_type;
			};

			template<typename _Res, typename... _ArgTypes>
			struct _Weak_result_type_impl<_Res(_ArgTypes......)> {
				typedef _Res result_type;
			};

			template<typename _Res, typename... _ArgTypes>
			struct _Weak_result_type_impl<_Res(_ArgTypes...) const> {
				typedef _Res result_type;
			};

			template<typename _Res, typename... _ArgTypes>
			struct _Weak_result_type_impl<_Res(_ArgTypes......) const> {
				typedef _Res result_type;
			};

			template<typename _Res, typename... _ArgTypes>
			struct _Weak_result_type_impl<_Res(_ArgTypes...) volatile> {
				typedef _Res result_type;
			};

			template<typename _Res, typename... _ArgTypes>
			struct _Weak_result_type_impl<_Res(_ArgTypes......) volatile> {
				typedef _Res result_type;
			};

			template<typename _Res, typename... _ArgTypes>
			struct _Weak_result_type_impl<_Res(_ArgTypes...) const volatile> {
				typedef _Res result_type;
			};

			template<typename _Res, typename... _ArgTypes>
			struct _Weak_result_type_impl<_Res(_ArgTypes......) const volatile> {
				typedef _Res result_type;
			};

			// Retrieve the result type for a function reference.
			template<typename _Res, typename... _ArgTypes>
			struct _Weak_result_type_impl<_Res(&)(_ArgTypes...)> {
				typedef _Res result_type;
			};

			template<typename _Res, typename... _ArgTypes>
			struct _Weak_result_type_impl<_Res(&)(_ArgTypes......)> {
				typedef _Res result_type;
			};

			// Retrieve the result type for a function pointer.
			template<typename _Res, typename... _ArgTypes>
			struct _Weak_result_type_impl<_Res(*)(_ArgTypes...)> {
				typedef _Res result_type;
			};

			template<typename _Res, typename... _ArgTypes>
			struct _Weak_result_type_impl<_Res(*)(_ArgTypes......)> {
				typedef _Res result_type;
			};

			// Retrieve result type for a member function pointer.
			template<typename _Res, typename _Class, typename... _ArgTypes>
			struct _Weak_result_type_impl<_Res (_Class::*)(_ArgTypes...)> {
				typedef _Res result_type;
			};

			template<typename _Res, typename _Class, typename... _ArgTypes>
			struct _Weak_result_type_impl<_Res (_Class::*)(_ArgTypes......)> {
				typedef _Res result_type;
			};

			// Retrieve result type for a const member function pointer.
			template<typename _Res, typename _Class, typename... _ArgTypes>
			struct _Weak_result_type_impl<_Res (_Class::*)(_ArgTypes...) const> {
				typedef _Res result_type;
			};

			template<typename _Res, typename _Class, typename... _ArgTypes>
			struct _Weak_result_type_impl<_Res (_Class::*)(_ArgTypes......) const> {
				typedef _Res result_type;
			};

			// Retrieve result type for a volatile member function pointer.
			template<typename _Res, typename _Class, typename... _ArgTypes>
			struct _Weak_result_type_impl<_Res (_Class::*)(_ArgTypes...) volatile> {
				typedef _Res result_type;
			};

			template<typename _Res, typename _Class, typename... _ArgTypes>
			struct _Weak_result_type_impl<_Res (_Class::*)(_ArgTypes......) volatile> {
				typedef _Res result_type;
			};

			// Retrieve result type for a const volatile member function pointer.
			template<typename _Res, typename _Class, typename... _ArgTypes>
			struct _Weak_result_type_impl<_Res (_Class::*)(_ArgTypes...)
			const volatile> {
				typedef _Res result_type;
			};

			template<typename _Res, typename _Class, typename... _ArgTypes>
			struct _Weak_result_type_impl<_Res (_Class::*)(_ArgTypes......)
			const volatile> {
				typedef _Res result_type;
			};

			/*
			 *  Strip top-level cv-qualifiers from the function object and let
			 *  _Weak_result_type_impl perform the real work.
			 */
			template<typename _Functor>
			struct _Weak_result_type : _Weak_result_type_impl<typename traits::RemoveCv<_Functor>::type> {
			};
		}

		template<typename _Tp>
		struct is_bind_expression : public traits::FalseType {
		};

		template<int _Num>
		struct _Placeholder {
		};

		template<typename _Tp>
		struct is_placeholder : public traits::IntegralConstant<int, 0> {
		};

		template<int _Num>
		struct is_placeholder<_Placeholder<_Num> > : public traits::IntegralConstant<int, _Num> {
		};

		template<int _Num>
		struct is_placeholder<const _Placeholder<_Num> > : public traits::IntegralConstant<int, _Num> {
		};


		namespace placeholders
		{
			extern const _Placeholder<1> _1;
			extern const _Placeholder<2> _2;
			extern const _Placeholder<3> _3;
			extern const _Placeholder<4> _4;
			extern const _Placeholder<5> _5;
			extern const _Placeholder<6> _6;
			extern const _Placeholder<7> _7;
			extern const _Placeholder<8> _8;
			extern const _Placeholder<9> _9;
			extern const _Placeholder<10> _10;
		}

		/* MU */
		namespace detail
		{
			struct _No_tuple_element;

			template<stl::size_t __i, typename _Tuple, bool _IsSafe>
			struct _Safe_tuple_element_impl : TupleElement<__i, _Tuple> {
			};

			template<stl::size_t __i, typename _Tuple>
			struct _Safe_tuple_element_impl<__i, _Tuple, false> {
				typedef _No_tuple_element Type;
				typedef _No_tuple_element type;
			};

			template<stl::size_t __i, typename _Tuple>
			struct _Safe_tuple_element : _Safe_tuple_element_impl<__i, _Tuple, (__i < TupleSize<_Tuple>::value)> {
			};


			template<typename _Arg,
					bool _IsBindExp = is_bind_expression<_Arg>::value,
					bool _IsPlaceholder = (is_placeholder<_Arg>::value > 0)>
			class _Mu;

			template<typename _Tp>
			class _Mu<ReferenceWrapper < _Tp> , false, false> {
			public:
				typedef _Tp &result_type;

				template<typename _CVRef, typename _Tuple>
				result_type
				operator()(_CVRef &__arg, _Tuple &) const volatile
				{
					return __arg.get();
				}
			};

			template<typename _Arg>
			class _Mu<_Arg, true, false> {
			public:
				template<typename _CVArg, typename... _Args>
				auto
				operator()(_CVArg &__arg,
						   Tuple<_Args...> &__tuple) const volatile
				-> decltype(__arg(traits::declval<_Args>()...))
				{
					// Construct an index tuple and forward to __call
					typedef typename BuildIndexTuple<sizeof...(_Args)>::__type
							_Indexes;
					return this->__call(__arg, __tuple, _Indexes());
				}

			private:
				// Invokes the underlying function object __arg by unpacking all
				// of the arguments in the tuple.
				template<typename _CVArg, typename... _Args, stl::size_t... _Indexes>
				auto
				__call(_CVArg &__arg, Tuple<_Args...> &__tuple,
					   const IndexTuple<_Indexes...> &) const volatile
				-> decltype(__arg(traits::declval<_Args>()...))
				{
					return __arg(stl::forward<_Args>(stl::get<_Indexes>(__tuple))...);
				}
			};

			template<typename _Arg>
			class _Mu<_Arg, false, false> {
			public:
				template<typename _Signature>
				struct result;

				template<typename _CVMu, typename _CVArg, typename _Tuple>
				struct result<_CVMu(_CVArg, _Tuple)> {
					typedef typename traits::AddLvalueReference<_CVArg>::type type;
				};

				// Pick up the cv-qualifiers of the argument
				template<typename _CVArg, typename _Tuple>
				_CVArg &&
				operator()(_CVArg &&__arg, _Tuple &) const volatile
				{
					return stl::forward<_CVArg>(__arg);
				}
			};

			template<typename _Arg>
			class _Mu<_Arg, false, true> {
			public:
				template<typename _Signature>
				class result;

				template<typename _CVMu, typename _CVArg, typename... Types>
				class result<_CVMu(_CVArg, Tuple<Types...>)> {
					// Add a reference, if it hasn't already been done for us.
					// This allows us to be a little bit sloppy in constructing
					// the tuple that we pass to result_of<...>.
//					typedef typename _Safe_tuple_element<is_placeholder<_Arg>::value - 1, Tuple<Types...>>::type __base_type;
					typedef  stl::type_at_index_t<is_placeholder<_Arg>::value - 1, Types...> __base_t;
//					typedef stl::TupleElement


				public:
					typedef typename traits::AddRvalueReference<__base_t>::type type;
				};


				template<typename... Types>
				typename result<_Mu(_Arg, Tuple<Types...>)>::type&&
//				int
				operator()(const volatile _Arg &, Tuple<Types...> &__tuple) const volatile
				{
					return stl::forward<typename result<_Mu(_Arg, Tuple<Types...>)>::type>(
//					return stl::forward<int>(
							stl::get<(is_placeholder<_Arg>::value - 1)>(__tuple));
				}
			};
		}

		namespace detail
		{
			template<stl::size_t _Ind, typename... _Tp>
			inline auto
			__volget(volatile Tuple<_Tp...> &__tuple)
			-> TupleElement_t <_Ind, Tuple<_Tp...>> volatile &
			{
				return stl::get<_Ind>(const_cast<Tuple < _Tp... > & > (__tuple));
			}

			template<stl::size_t _Ind, typename... _Tp>
			inline auto
			__volget(const volatile Tuple<_Tp...> &__tuple)
			-> TupleElement_t <_Ind, Tuple<_Tp...>> const volatile &
			{
				return stl::get<_Ind>(const_cast<const Tuple < _Tp... > & > (__tuple));
			}

			template<typename _Signature>
			class _Bind;

			template<typename _Functor, typename... _Bound_args>
			class _Bind<_Functor(_Bound_args...)>
					: public _Weak_result_type<_Functor> {
				typedef _Bind __self_type;
				typedef typename BuildIndexTuple<sizeof...(_Bound_args)>::__type
						_Bound_indexes;

				_Functor _M_f;
				Tuple<_Bound_args...> _M_bound_args;

				// Call unqualified
				template<typename _Result, typename... _Args, stl::size_t... _Indexes>
				_Result
				__call(Tuple<_Args...> &&__args, IndexTuple<_Indexes...>)
				{
					return _M_f(_Mu<_Bound_args>()(stl::get<_Indexes>(_M_bound_args), __args)...);
				}

				// Call as const
				template<typename _Result, typename... _Args, stl::size_t... _Indexes>
				_Result
				__call_c(Tuple<_Args...> &&__args, IndexTuple<_Indexes...>) const
				{
					return _M_f(_Mu<_Bound_args>()(stl::get<_Indexes>(_M_bound_args), __args)...);
				}

				// Call as volatile
				template<typename _Result, typename... _Args, stl::size_t... _Indexes>
				_Result
				__call_v(Tuple<_Args...> &&__args, IndexTuple<_Indexes...>) volatile
				{
					return _M_f(_Mu<_Bound_args>()(__volget<_Indexes>(_M_bound_args), __args)...);
				}

				// Call as const volatile
				template<typename _Result, typename... _Args, stl::size_t... _Indexes>
				_Result __call_c_v(Tuple<_Args...> &&__args, IndexTuple<_Indexes...>) const volatile
				{
					return _M_f(_Mu<_Bound_args>()(__volget<_Indexes>(_M_bound_args), __args)...);
				}

			public:
				template<typename... _Args>
				explicit _Bind(const _Functor &__f, _Args &&... __args)
						: _M_f(__f), _M_bound_args(stl::forward<_Args>(__args)...)
				{
				}

				template<typename... _Args>
				explicit _Bind(_Functor &&__f, _Args &&... __args)
						: _M_f(stl::move(__f)), _M_bound_args(stl::forward<_Args>(__args)...)
				{
				}

				_Bind(const _Bind &) = default;

				_Bind(_Bind &&__b) noexcept
						: _M_f(stl::move(__b._M_f)), _M_bound_args(stl::move(__b._M_bound_args))
				{
				}

				// Call unqualified
				template<typename... _Args, typename _Result
				= decltype( traits::declval<_Functor>()(
						_Mu<_Bound_args>()( traits::declval<_Bound_args&>(),
						                    traits::declval<stl::Tuple<_Args...>&>() )... ) )>
				_Result operator()(_Args&&... __args)
				{
					return this->__call<_Result>(stl::ForwardAsTuple(stl::forward<_Args>(__args)...), _Bound_indexes());
				}

				// Call as const
				template<typename... _Args, typename _Result
				= decltype(traits::declval<typename traits::EnableIf<(sizeof...(_Args) >= 0),
						typename traits::AddConst<_Functor>::type>::type>()(
						_Mu<_Bound_args>()(traits::declval<const _Bound_args &>(),
										   traits::declval < Tuple < _Args... > & > ())...))>

				_Result
				operator()(_Args &&... __args) const
				{
					return this->__call_c<_Result>(
							stl::ForwardAsTuple(stl::forward<_Args>(__args)...),
							_Bound_indexes());
				}

				// Call as volatile
				template<typename... _Args, typename _Result
				= decltype(traits::declval<typename traits::EnableIf<(sizeof...(_Args) >= 0),
						typename traits::AddVolatile<_Functor>::type>::type>()(
						_Mu<_Bound_args>()(traits::declval<volatile _Bound_args &>(),
										   traits::declval < Tuple < _Args... > & > ())...))>

				_Result
				operator()(_Args &&... __args) volatile
				{
					return this->__call_v<_Result>(stl::ForwardAsTuple(stl::forward<_Args>(__args)...),
												   _Bound_indexes());
				}

				// Call as const volatile
				template<typename... _Args, typename _Result
				= decltype(traits::declval<typename traits::EnableIf<(sizeof...(_Args) >= 0),
						typename traits::AddCv<_Functor>::type &>::type>()(
						_Mu<_Bound_args>()(traits::declval<const volatile _Bound_args &>(),
										   traits::declval < Tuple < _Args... > & > ())...))>

				_Result
				operator()(_Args &&... __args) const volatile
				{
					return this->__call_c_v<_Result>(
							stl::ForwardAsTuple(stl::forward<_Args>(__args)...),
							_Bound_indexes());
				}
			};

			template<typename T>
			using is_void = traits::IsSame<T, void>;

			template<typename _Result, typename _Signature>
			class _Bind_result;

			template<typename _Result, typename _Functor, typename... _Bound_args>
			class _Bind_result<_Result, _Functor(_Bound_args...)> {
				typedef _Bind_result __self_type;
				typedef typename BuildIndexTuple<sizeof...(_Bound_args)>::__type
						_Bound_indexes;

				_Functor _M_f;
				Tuple<_Bound_args...> _M_bound_args;

				// sfinae types
				template<typename _Res>
				struct __enable_if_void : traits::EnableIf<is_void<_Res>::value, int> {
				};
				template<typename _Res>
				struct __disable_if_void : traits::EnableIf<!is_void<_Res>::value, int> {
				};

				// Call unqualified
				template<typename _Res, typename... _Args, stl::size_t... _Indexes>
				_Result
				__call(Tuple<_Args...> &&__args, IndexTuple<_Indexes...>,
					   typename __disable_if_void<_Res>::type = 0)
				{
					return _M_f(_Mu<_Bound_args>()(stl::get<_Indexes>(_M_bound_args), __args)...);
				}

				// Call unqualified, return void
				template<typename _Res, typename... _Args, stl::size_t... _Indexes>
				void
				__call(Tuple<_Args...> &&__args, IndexTuple<_Indexes...>,
					   typename __enable_if_void<_Res>::type = 0)
				{
					_M_f(_Mu<_Bound_args>()(stl::get<_Indexes>(_M_bound_args), __args)...);
				}

				// Call as const
				template<typename _Res, typename... _Args, stl::size_t... _Indexes>
				_Result __call(Tuple<_Args...> &&__args, IndexTuple<_Indexes...>,
							   typename __disable_if_void<_Res>::type = 0) const
				{
					return _M_f(_Mu<_Bound_args>()
										(stl::get<_Indexes>(_M_bound_args), __args)...);
				}

				// Call as const, return void
				template<typename _Res, typename... _Args, stl::size_t... _Indexes>
				void __call(Tuple<_Args...> &&__args, IndexTuple<_Indexes...>, typename __enable_if_void<_Res>::type = 0) const
				{
					_M_f(_Mu<_Bound_args>()
								 (stl::get<_Indexes>(_M_bound_args), __args)...);
				}

				// Call as volatile
				template<typename _Res, typename... _Args, stl::size_t... _Indexes>
				_Result
				__call(Tuple<_Args...> &&__args, IndexTuple<_Indexes...>,
					   typename __disable_if_void<_Res>::type = 0) volatile
				{
					return _M_f(_Mu<_Bound_args>()
										(__volget<_Indexes>(_M_bound_args), __args)...);
				}

				// Call as volatile, return void
				template<typename _Res, typename... _Args, stl::size_t... _Indexes>
				void
				__call(Tuple<_Args...> &&__args, IndexTuple<_Indexes...>,
					   typename __enable_if_void<_Res>::type = 0) volatile
				{
					_M_f(_Mu<_Bound_args>()
								 (__volget<_Indexes>(_M_bound_args), __args)...);
				}

				// Call as const volatile
				template<typename _Res, typename... _Args, stl::size_t... _Indexes>
				_Result
				__call(Tuple<_Args...> &&__args, IndexTuple<_Indexes...>,
					   typename __disable_if_void<_Res>::type = 0) const volatile
				{
					return _M_f(_Mu<_Bound_args>()
										(__volget<_Indexes>(_M_bound_args), __args)...);
				}

				// Call as const volatile, return void
				template<typename _Res, typename... _Args, stl::size_t... _Indexes>
				void
				__call(Tuple<_Args...> &&__args,
					   IndexTuple<_Indexes...>,
					   typename __enable_if_void<_Res>::type = 0) const volatile
				{
					_M_f(_Mu<_Bound_args>()
								 (__volget<_Indexes>(_M_bound_args), __args)...);
				}

			public:
				typedef _Result result_type;

				template<typename... _Args>
				explicit _Bind_result(const _Functor &__f, _Args &&... __args)
						: _M_f(__f), _M_bound_args(stl::forward<_Args>(__args)...)
				{
				}

				template<typename... _Args>
				explicit _Bind_result(_Functor &&__f, _Args &&... __args)
						: _M_f(stl::move(__f)), _M_bound_args(stl::forward<_Args>(__args)...)
				{
				}

				_Bind_result(const _Bind_result &) = default;

				_Bind_result(_Bind_result &&__b) noexcept :
						_M_f(stl::move(__b._M_f)), _M_bound_args(stl::move(__b._M_bound_args))
				{
				}

				// Call unqualified
				template<typename... _Args>
				result_type
				operator()(_Args &&... __args)
				{
					return this->__call<_Result>(
							stl::ForwardAsTuple(stl::forward<_Args>(__args)...),
							_Bound_indexes());
				}

				// Call as const
				template<typename... _Args>
				result_type
				operator()(_Args &&... __args) const
				{
					return this->__call<_Result>(
							stl::ForwardAsTuple(stl::forward<_Args>(__args)...),
							_Bound_indexes());
				}

				// Call as volatile
				template<typename... _Args>
				result_type
				operator()(_Args &&... __args) volatile
				{
					return this->__call<_Result>(
							stl::ForwardAsTuple(stl::forward<_Args>(__args)...),
							_Bound_indexes());
				}

				// Call as const volatile
				template<typename... _Args>
				result_type
				operator()(_Args &&... __args) const volatile
				{
					return this->__call<_Result>(
							stl::ForwardAsTuple(stl::forward<_Args>(__args)...),
							_Bound_indexes());
				}
			};


			template<typename _Tp>
			struct _Maybe_wrap_member_pointer {
				typedef _Tp type;

				static const _Tp &
				__do_wrap(const _Tp &__x)
				{
					return __x;
				}

				static _Tp &&
				__do_wrap(_Tp &&__x)
				{
					return static_cast<_Tp &&>(__x);
				}
			};

			template<typename _Tp, typename _Class>
			struct _Maybe_wrap_member_pointer<_Tp _Class::*> {
				typedef _Mem_fn<_Tp _Class::*> type;

				static type
				__do_wrap(_Tp _Class::* __pm)
				{
					return type(__pm);
				}
			};

			template<>
			struct _Maybe_wrap_member_pointer<void> {
				typedef void type;
			};

			template<typename _Func, typename... _BoundArgs>
			struct _Bind_check_arity {
			};

			template<typename _Ret, typename... _Args, typename... _BoundArgs>
			struct _Bind_check_arity<_Ret (*)(_Args...), _BoundArgs...> {
				static_assert(sizeof...(_BoundArgs) == sizeof...(_Args),
							  "Wrong number of arguments for function");
			};

			template<typename _Ret, typename... _Args, typename... _BoundArgs>
			struct _Bind_check_arity<_Ret (*)(_Args......), _BoundArgs...> {
				static_assert(sizeof...(_BoundArgs) >= sizeof...(_Args),
							  "Wrong number of arguments for function");
			};

			template<typename _Tp, typename _Class, typename... _BoundArgs>
			struct _Bind_check_arity<_Tp _Class::*, _BoundArgs...> {
				using _Arity = typename _Mem_fn<_Tp _Class::*>::_Arity;
				using _Varargs = typename _Mem_fn<_Tp _Class::*>::_Varargs;
				static_assert(_Varargs::value
							  ? sizeof...(_BoundArgs) >= _Arity::value + 1
							  : sizeof...(_BoundArgs) == _Arity::value + 1,
							  "Wrong number of arguments for pointer-to-member");
			};
		}

		template<typename _Signature>
		struct is_bind_expression<detail::_Bind<_Signature> > : public traits::TrueType {
		};

		template<typename _Signature>
		struct is_bind_expression<const detail::_Bind<_Signature> > : public traits::TrueType {
		};

		template<typename _Signature>
		struct is_bind_expression<volatile detail::_Bind<_Signature> > : public traits::TrueType {
		};

		template<typename _Signature>
		struct is_bind_expression<const volatile detail::_Bind<_Signature>> : public traits::TrueType {
		};

		template<typename _Result, typename _Signature>
		struct is_bind_expression<detail::_Bind_result<_Result, _Signature>> : public traits::TrueType {
		};

		template<typename _Result, typename _Signature>
		struct is_bind_expression<const detail::_Bind_result<_Result, _Signature>> : public traits::TrueType {
		};

		template<typename _Result, typename _Signature>
		struct is_bind_expression<volatile detail::_Bind_result<_Result, _Signature>> : public traits::TrueType {
		};

		template<typename _Result, typename _Signature>
		struct is_bind_expression<const volatile detail::_Bind_result<_Result, _Signature>> : public traits::TrueType {
		};

		template<typename _Tp, typename _Tp2 = typename traits::Decay<_Tp>::type>
		using __is_socketlike = traits::Or <traits::IsIntegral<_Tp2>, traits::IsEnum<_Tp2>>;

		template<bool _SocketLike, typename _Func, typename... _BoundArgs>
		struct _Bind_helper : detail::_Bind_check_arity<typename traits::Decay<_Func>::type, _BoundArgs...> {
			typedef detail::_Maybe_wrap_member_pointer<typename traits::Decay<_Func>::type> __maybe_type;
			typedef typename __maybe_type::type __func_type;
			typedef detail::_Bind<__func_type(typename traits::Decay<_BoundArgs>::type...)> type;
		};

		template<typename _Func, typename... _BoundArgs>
		struct _Bind_helper<true, _Func, _BoundArgs...> {
		};

		/**
		 * @brief Generate a forwarding call wrapper for \p f. Some arguments can be bound.
		 * @tparam _Func Type of the callable.
		 * @tparam _BoundArgs Argument types bound to \p __f.
		 * @param __f Functor.
		 * @param __args Arguments to \p __f.
		 * @return A callable object.
		 */
		template<typename _Func, typename... _BoundArgs>
		inline typename
		_Bind_helper<__is_socketlike<_Func>::value, _Func, _BoundArgs...>::type
		bind(_Func &&__f, _BoundArgs &&... __args)
		{
			typedef _Bind_helper<false, _Func, _BoundArgs...> __helper_type;
			typedef typename __helper_type::__maybe_type __maybe_type;
			typedef typename __helper_type::type __result_type;
			return __result_type(__maybe_type::__do_wrap(stl::forward<_Func>(__f)),
								 stl::forward<_BoundArgs>(__args)...);
		}

		template<typename _Result, typename _Func, typename... _BoundArgs>
		struct _Bindres_helper : detail::_Bind_check_arity<typename traits::Decay<_Func>::type, _BoundArgs...> {
			typedef detail::_Maybe_wrap_member_pointer<typename traits::Decay<_Func>::type>
					__maybe_type;
			typedef typename __maybe_type::type __functor_type;
			typedef detail::_Bind_result<_Result, __functor_type(typename traits::Decay<_BoundArgs>::type...)> type;
		};


		/**
		 * @brief Generate a forwarding call wrapper for \p f. Some arguments can be bound.
		 * @tparam _Result Result type.
		 * @tparam _Func Type of the callable.
		 * @tparam _BoundArgs Argument types bound to \p __f.
		 * @param __f Functor.
		 * @param __args Arguments to \p __f.
		 * @return A callable object.
		 */
		template<typename _Result, typename _Func, typename... _BoundArgs>
		inline typename _Bindres_helper<_Result, _Func, _BoundArgs...>::type
		bind(_Func &&__f, _BoundArgs &&... __args)
		{
			typedef _Bindres_helper<_Result, _Func, _BoundArgs...> __helper_type;
			typedef typename __helper_type::__maybe_type __maybe_type;
			typedef typename __helper_type::type __result_type;
			return __result_type(__maybe_type::__do_wrap(stl::forward<_Func>(__f)),
								 stl::forward<_BoundArgs>(__args)...);
		}

	}
}

/// @ }
