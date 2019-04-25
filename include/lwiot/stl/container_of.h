/*
 * C++ implementation of container_of.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stddef.h>

namespace lwiot { namespace stl
{
	template <typename T, typename M>
	constexpr ptrdiff_t offset_of(const M T::*member)
	{
		return reinterpret_cast<ptrdiff_t>( &(reinterpret_cast<T*>(0)->*member) );
	}

	template <typename T, typename M>
	constexpr T* owner_of(M* ptr, const M T::*member)
	{
		return reinterpret_cast< T* >( reinterpret_cast< intptr_t >( ptr ) - offset_of( member ) );
	}
}}
