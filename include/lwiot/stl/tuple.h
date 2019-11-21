/*
 * STL tuple implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

/**
 * @file tuple.h
 */

#include <stdlib.h>
#include <stddef.h>

#include <lwiot/types.h>
#include <lwiot/stl/forward.h>
#include <lwiot/stl/move.h>
#include <lwiot/stl/referencewrapper.h>
#include <lwiot/stl/integersequence.h>

#include <lwiot/traits/decay.h>
#include <lwiot/traits/removecv.h>
#include <lwiot/traits/enableif.h>

#include <lwiot/traits/integralconstant.h>

namespace lwiot
{
	namespace stl
	{
		template<size_t... idx>
		using IndexSequence = IntegerSequence<size_t, idx...>;

		template<size_t I, typename Sequence>
		struct cat_index_sequence;

		template<size_t I, size_t... Indices>
		struct cat_index_sequence<I, IndexSequence<Indices...>> : IndexSequence<Indices..., I> {
		};

		template<size_t N>
		struct make_index_sequence
				: cat_index_sequence<N - 1, typename make_index_sequence<N - 1>::type>::type {
		};

		template<>
		struct make_index_sequence<1> : IndexSequence<0> {
		};

		namespace detail
		{
			template<typename... T>
			struct TemplateForAll;

			template<>
			struct TemplateForAll<> : public traits::TrueType {
			};

			template<typename... T>
			struct TemplateForAll<traits::FalseType, T...> : public traits::FalseType {
			};

			template<typename... T>
			struct TemplateForAll<traits::TrueType, T...> : public TemplateForAll<T...>::type {
			};
		}

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

		template<template<class> typename>
		constexpr bool is_any_of()
		{
			return false;
		}

		template<template<class> typename Op, typename Head, typename... Tail>
		constexpr bool is_any_of()
		{
			return Op<Head>::value || is_any_of<Op, Tail...>();
		}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

		template<typename ... Types>
		class Tuple;

		template<size_t, typename T>
		struct TupleElement {
			typedef T Type;
			typedef T type;
			Type _value;

			explicit TupleElement(const T& value) : _value(value) { }

			template<typename U>
			TupleElement(U &&value) noexcept : _value(stl::forward<U>(value))
			{
			}
		};


		template<typename seq, typename ... T>
		struct TupleImpl;

		template<typename>
		struct IsTupleImpl : traits::FalseType {
		};

		template<size_t ... Indices, typename ... Types>
		struct IsTupleImpl<TupleImpl<IndexSequence<Indices...>, Types...>> : traits::TrueType {
		};

		template<size_t... indices, typename... Types>
		struct TupleImpl<IndexSequence<indices...>, Types...> : public TupleElement<indices, Types> ... {
			template<typename... OtherTypes,
					typename = typename traits::EnableIf<sizeof...(OtherTypes) == sizeof...(Types) &&
					                                     !is_any_of<IsTupleImpl, typename traits::Decay<OtherTypes>::type...>()
					>::type>
			TupleImpl(OtherTypes &&... elems) noexcept : TupleElement<indices, Types>(stl::forward<OtherTypes>(elems))...
			{
			}
		};


		/**
		 * @brief Tuple object.
		 * @tparam Types Types wrapped by the tuple.
		 */
		template<typename ... Types>
		class Tuple : public TupleImpl<typename make_index_sequence<sizeof...(Types)>::type, Types...> {
		private:
			using tuple_base_t = TupleImpl<typename make_index_sequence<sizeof...(Types)>::type, Types...>;

			template <typename... Args>
			static constexpr bool IsTuple()
			{
				return detail::TemplateForAll<typename traits::IsSpecializationOf<Args, stl::Tuple>::type...>::value;
			}

		public:
			explicit Tuple() = default;

			Tuple(Tuple const &) = default;

			template<typename Dummy = void, typename = typename traits::EnableIf<sizeof...(Types) >= 1, bool>::type>
			Tuple(const Types& ...types) : TupleImpl<typename make_index_sequence<sizeof...(Types)>::type, Types...>(types...)
			{
			}

			Tuple(Tuple &&) noexcept = default;

			template<typename ... OtherTypes, typename traits::EnableIf<sizeof...(OtherTypes) == sizeof...(Types) &&
			        sizeof...(Types) >= 1 && !IsTuple<typename traits::Decay<OtherTypes>::type...>(), bool>::type = true>
			Tuple(OtherTypes &&... elems) noexcept : tuple_base_t(stl::forward<OtherTypes>(elems)...)
			{
			}


			Tuple &operator=(Tuple const &rhs) = default;

			Tuple &operator=(Tuple &&) noexcept = default;
		};

		template<>
		class Tuple<> {
		};


		template<size_t I, typename, typename ... Tail>
		struct TypeAtIndex {
			using type = typename TypeAtIndex<I - 1, Tail...>::type;
		};

		template<typename Head, typename ... Tail>
		struct TypeAtIndex<0, Head, Tail...> {
			using type = Head;
		};

		template<size_t I, typename... Types>
		using type_at_index_t = typename TypeAtIndex<I, Types...>::type;

		template<typename>
		constexpr int count()
		{
			return 0;
		}

		template<typename T, typename Head, typename... Tail>
		constexpr int count()
		{
			return (traits::IsSame<T, Head>::value ? 1 : 0) + count<T, Tail...>();
		}

		template<typename>
		constexpr int find(int idx)
		{
			return -1;
		}

		template<typename T, typename Head, typename ... Tail>
		constexpr int find(int idx = 0)
		{
			return traits::IsSame<T, Head>::value ? idx : find<T, Tail...>(idx + 1);
		}

		template<typename T, typename ... Types>
		T &get(Tuple<Types...> &tuple)
		{
			static_assert(count<T, Types...>() == 1, "T must appear exactly once in the tuple!");
			constexpr auto idx = find<T, Types...>();
			return get<idx>(tuple);
		}

		template<size_t I, typename ... Types>
		constexpr typename traits::RemoveReference<type_at_index_t<I, Types...>>::type &&get(Tuple<Types...> &&tuple)
		{
			TupleElement<I, type_at_index_t<I, Types...>> &base = tuple;
			return stl::move(base._value);
		}

		template<size_t I, typename... Types>
		constexpr type_at_index_t<I, Types...> &get(Tuple<Types...> &tup)
		{
			TupleElement<I, type_at_index_t<I, Types...>> &base = tup;
			return base._value;
		}

		template<size_t I, typename... Types>
		constexpr const type_at_index_t<I, Types...> &get(const Tuple<Types...> &tup)
		{
			const TupleElement<I, type_at_index_t<I, Types...>> &base = tup;
			return base._value;
		}

		template<size_t __i, typename _Tp>
		using TupleElement_t = typename TupleElement<__i, _Tp>::Type ;

		/* Size calculation */
		template<typename>
		struct TupleSize;

		template<typename ... Types>
		struct TupleSize<Tuple<Types...>> : traits::IntegralConstant<size_t, sizeof...(Types)> {
		};

		template<typename T>
		struct UnwrapRefwrapper {
			using type = T;
		};

		template<typename Ref>
		struct UnwrapRefwrapper<stl::ReferenceWrapper<Ref>> {
			using type = Ref &;
		};

		template<class T>
		using special_decay_t = typename UnwrapRefwrapper<typename traits::Decay<T>::type>::type;

		template<class T>
		using SpecialDecay = UnwrapRefwrapper<typename traits::Decay<T>::type>;

		template<typename... _Elements>
		constexpr Tuple<typename SpecialDecay<_Elements>::type...>
		MakeTuple(_Elements&&... __args)
		{
			typedef Tuple<typename SpecialDecay<_Elements>::type...> __result_type;
			return __result_type(stl::forward<_Elements>(__args)...);
		}

		template <typename... T>
		constexpr Tuple<T&...> tie(T&... args)
		{
			return Tuple<T&...>(args...);
		}

		template<size_t... Indices>
		struct IndexTuple {
			typedef IndexTuple<Indices..., sizeof...(Indices)> __next;
		};

		template <size_t _Num>
		struct BuildIndexTuple {
			typedef typename BuildIndexTuple<_Num - 1>::__type::__next __type;
		};

		template <>
		struct BuildIndexTuple<0> {
			typedef IndexTuple<> __type;
		};

		template<typename... _Elements>
		constexpr Tuple<_Elements&&...> ForwardAsTuple(_Elements&&... __args) noexcept
		{
			return Tuple<_Elements&&...>(stl::forward<_Elements>(__args)...);
		}
	}
}

