/*
 * STL tuple implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once


#include <stdlib.h>
#include <stddef.h>

#include <lwiot/types.h>
#include <lwiot/stl/forward.h>
#include <lwiot/stl/move.h>
#include <lwiot/stl/referencewrapper.h>
#include <lwiot/stl/integersequence.h>

#include <lwiot/traits/decay.h>
#include <lwiot/traits/enableif.h>

namespace lwiot
{
	namespace stl
	{
		template<size_t... idx>
		using IndexSequence = IntegerSequence<size_t, idx...>;

		template<size_t I, typename Sequence>
		struct cat_index_sequence;

		template<size_t I, size_t... Indices>
		struct cat_index_sequence<I, IndexSequence < Indices...>> : IndexSequence <Indices..., I>
		{
		};

		template<size_t N>
		struct make_index_sequence
				: cat_index_sequence<N - 1, typename make_index_sequence<N - 1>::type>::type {
		};

		template<>
		struct make_index_sequence<1> : IndexSequence <0> {
		};

		template<size_t, typename T>
		struct TupleElement {
			typedef T Type;
			Type _value;


			explicit TupleElement(T const& value) : _value(value) {}
			explicit TupleElement(T&& value) : _value(stl::move(value)) {}

			template <typename U>
			explicit TupleElement(U&& value) : _value(stl::forward<U>(value))
			{ }
		};


		template<typename seq, typename ... T>
		struct TupleImpl;

		template <typename >
		struct IsTupleImpl : traits::FalseType
		{ };

		template <size_t ... Indices, typename ... Types>
		struct IsTupleImpl<TupleImpl<IndexSequence <Indices...>, Types...>> : traits::TrueType
		{ };

#ifndef WIN32
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

		template <template <class> typename>
		constexpr bool is_any_of()
		{
			return false;
		}

		template <template <class> typename Op, typename Head, typename... Tail>
		constexpr bool is_any_of()
		{
			return Op<Head>::value || is_any_of<Op, Tail...>();
		}

#ifndef WIN32
#pragma GCC diagnostic pop
#endif

		template<size_t... indices, typename... types>
		struct TupleImpl<IndexSequence < indices...>, types...> : public TupleElement<indices, types>... {
			template <typename... OtherTypes,
					typename = typename traits::EnableIf<
					        !is_any_of<IsTupleImpl, typename traits::Decay<OtherTypes>::type...>()
					        >::type >
			explicit TupleImpl(OtherTypes&&... elems) : TupleElement<indices, types>(stl::forward<OtherTypes>(elems))...
			{

			}
		};


		template <typename ... Types>
		class Tuple : public TupleImpl<typename make_index_sequence<sizeof...(Types)>::type, Types...> {
		private:
			using tuple_base_t = TupleImpl<typename make_index_sequence<sizeof...(Types)>::type , Types...>;

		public:
			explicit Tuple() = default;
			 Tuple(Tuple const&) = default;
			 Tuple(Tuple&&) = default;

			template <typename ... OtherTypes,
			        typename = typename traits::EnableIf<
			                sizeof...(OtherTypes) == sizeof...(Types)>::type
		                >
			explicit Tuple(OtherTypes&&... elems) : tuple_base_t (stl::forward<OtherTypes>(elems)...)
			{ }


			Tuple& operator=(Tuple const& rhs) = default;
			Tuple& operator=(Tuple&&) = default;
		};

		template <size_t I, typename, typename ... Tail>
		struct TypeAtIndex {
			using type = typename TypeAtIndex<I-1, Tail...>::type;
		};

		template <typename Head, typename ... Tail>
		struct TypeAtIndex<0, Head, Tail...> {
			using type = Head;
		};

		template <size_t I, typename... Types>
		using type_at_index_t = typename TypeAtIndex<I, Types...>::type ;

		template <typename >
		constexpr int count()
		{
			return 0;
		}

		template <typename T, typename Head, typename... Tail>
		constexpr int count()
		{
			return (traits::IsSame<T, Head>::value ? 1 : 0) + count<T, Tail...>();
		}

		template <typename >
		constexpr int find(int idx)
		{
			return -1;
		}

		template <typename T, typename Head, typename ... Tail>
		constexpr int find(int idx = 0)
		{
			return traits::IsSame<T,Head>::value ? idx : find<T, Tail...>(idx + 1);
		}

		template <typename T, typename ... Types>
		T& get(Tuple<Types...>& tuple)
		{
			static_assert(count<T, Types...>() == 1, "T must appear exactly once in the tuple!");
			constexpr auto idx = find<T, Types...>();
			return get<idx>(tuple);
		}

		template <size_t I, typename ... Types>
		constexpr typename traits::RemoveReference<type_at_index_t<I, Types...>>::type && get(Tuple<Types...>&& tuple)
		{
			TupleElement<I, type_at_index_t<I, Types...>>& base = tuple;
			return stl::move(base._value);
		}

		template <size_t I, typename... Types>
		constexpr type_at_index_t<I, Types...> const& get(Tuple<Types...> const& tup)
		{
			TupleElement<I, type_at_index_t<I, Types...>>& base = tup;
			return base._value;
		}

		template <size_t I, typename... Types>
		constexpr type_at_index_t<I, Types...>& get(Tuple<Types...>& tup)
		{
			TupleElement<I, type_at_index_t<I, Types...>>& base = tup;
			return base._value;
		}

		/* Size calculation */
		template <typename >
		struct TupleSize;

		template <typename ... Types>
		struct TupleSize<Tuple<Types...>> : traits::IntegralConstant<size_t, sizeof...(Types)> {
		};

		template <typename T>
		struct UnwrapRefwrapper {
			using type = T;
		};

		template <typename Ref>
		struct UnwrapRefwrapper<stl::ReferenceWrapper<Ref>> {
			using type = Ref&;
		};

		template <class T>
		using special_decay_t = typename UnwrapRefwrapper<typename traits::Decay<T>::type>::type ;

		template <class... Types>
		constexpr Tuple<special_decay_t<Types>...> MakeTuple(Types&&... args)
		{
			return lwiot::stl::Tuple<special_decay_t<Types>...>(stl::forward<Types>(args)...);
		}

/*template<typename Value>
struct Tuple<Value> {
	constexpr Tuple()
	{
	}

	constexpr Tuple(const Tuple &tuple) : value(tuple.value)
	{
	}

	constexpr Tuple(Tuple &&tuple) : value(stl::move(tuple.value))
	{
	}

	constexpr Tuple(Value value) : value(value)
	{
	}

	Tuple &operator=(const Tuple &) = default;

	Value value;
};*/


/*template<int index, typename Value, typename... Rest>
struct GetImpl {
	static auto value(const Tuple<Value, Rest...> *t) -> decltype(GetImpl<index - 1, Rest...>::value(t))
	{
		return GetImpl<index - 1, Rest...>::value(t);
	}
};

template<typename Value, typename... Rest>
struct GetImpl<0, Value, Rest...> {
	static Value value(const Tuple<Value, Rest...> *t)
	{
		return t->value;
	}
};

template<int index, typename Value, typename... Rest>
auto get(const Tuple<Value, Rest...> &t) -> decltype(GetImpl<index, Value, Rest...>::value(&t))
{
	return GetImpl<index, Value, Rest...>::value(&t);
}*/

	}
}

