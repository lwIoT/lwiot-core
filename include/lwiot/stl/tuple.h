/*
 * STL tuple implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/stl/forward.h>
#include <lwiot/stl/referencewrapper.h>
#include <lwiot/traits/decay.h>

namespace lwiot
{
	namespace stl
	{
		template<typename Value, typename... Rest>
		struct Tuple : public Tuple<Rest...> {
			constexpr Tuple() : Tuple<Rest...>()
			{
			}

			constexpr Tuple(const Tuple &tuple) : Tuple<Rest...>(tuple), value(tuple.value)
			{
			}

			constexpr Tuple(Tuple &&tuple) : Tuple<Rest...>(tuple), value(stl::move(tuple.value))
			{
			}

			constexpr Tuple(Value value, Rest... rest) : Tuple<Rest...>(rest...), value(value)
			{
			}

			Tuple &operator=(const Tuple &) = default;

			Value value;
		};

		template<typename Value>
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
		};

		template<int index, typename Value, typename... Rest>
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
		}

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
		constexpr auto MakeTuple(Types&&... args)
		{
			return lwiot::stl::Tuple<special_decay_t<Types>...>(stl::forward<Types>(args)...);
		}
	}
}
