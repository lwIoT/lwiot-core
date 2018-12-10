/*
 * Move data by value.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <lwiot/traits/removereference.h>
#include <lwiot/stl/forward.h>

namespace lwiot { namespace stl {
		template<typename _Tp>
		constexpr typename traits::RemoveReference<_Tp>::type&& move(_Tp&& __t) noexcept
		{
			return static_cast<typename traits::RemoveReference<_Tp>::type &&>(__t);
		}

		template <typename T, typename V = T>
		constexpr T __exchange(T& obj, V&& nobj)
		{
			T old = move(obj);
			obj = forward(nobj);
			return old;
		}

		template <typename T, typename V = T>
		constexpr void swap(T& a, V& b)
		{
			b = move(__exchange(a,b));
		}
} }
