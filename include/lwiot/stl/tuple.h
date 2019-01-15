/*
 * STL tuple implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

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
	}
}