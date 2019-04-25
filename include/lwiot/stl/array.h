/*
 * Wrapper class around C-style arrays.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/traits/integralconstant.h>

namespace lwiot
{
	namespace stl
	{
		template<typename _Tp, size_t _Nm>
		struct __array_traits {
			typedef _Tp _Type[_Nm];

			static constexpr _Tp &_S_ref(const _Type &__t, size_t __n) noexcept
			{
				return const_cast<_Tp &>(__t[__n]);
			}

			static constexpr _Tp *_S_ptr(const _Type &__t) noexcept
			{
				return const_cast<_Tp *>(__t);
			}
		};

		template<typename _Tp>
		struct __array_traits<_Tp, 0> {
			struct _Type {
			};
			typedef traits::TrueType _Is_swappable;
			typedef traits::TrueType _Is_nothrow_swappable;

			static constexpr _Tp &
			_S_ref(const _Type &, size_t) noexcept
			{
				return *static_cast<_Tp *>(nullptr);
			}

			static constexpr _Tp *
			_S_ptr(const _Type &) noexcept
			{
				return nullptr;
			}
		};

		template<typename _Tp, size_t _Nm>
		struct Array {
			typedef _Tp value_type;
			typedef value_type *pointer;
			typedef const value_type *const_pointer;
			typedef value_type &reference;
			typedef const value_type &const_reference;
			typedef value_type *iterator;
			typedef const value_type *const_iterator;
			typedef size_t size_type;
			typedef ptrdiff_t difference_type;

			// Support for zero-sized arrays mandatory.
			typedef lwiot::stl::__array_traits<_Tp, _Nm> _AT_Type;
			typename _AT_Type::_Type _M_elems;

			// No explicit construct/copy/destroy for aggregate type.

			CONSTEXPR iterator begin() noexcept
			{
				return iterator(data());
			}

			CONSTEXPR const_iterator begin() const noexcept
			{
				return const_iterator(data());
			}

			CONSTEXPR iterator end() noexcept
			{
				return iterator(data() + _Nm);
			}

			CONSTEXPR const_iterator end() const noexcept
			{
				return const_iterator(data() + _Nm);
			}

			CONSTEXPR const_iterator cbegin() const noexcept
			{
				return const_iterator(data());
			}

			CONSTEXPR const_iterator cend() const noexcept
			{
				return const_iterator(data() + _Nm);
			}

			constexpr size_type size() const noexcept
			{
				return _Nm;
			}

			constexpr size_type max_size() const noexcept
			{
				return _Nm;
			}

			constexpr bool empty() const noexcept
			{
				return size() == 0;
			}

			// Element access.
			CONSTEXPR reference operator[](size_type __n) noexcept
			{
				return _AT_Type::_S_ref(_M_elems, __n);
			}

			CONSTEXPR const_reference operator[](size_type __n) const noexcept
			{
				return _AT_Type::_S_ref(_M_elems, __n);
			}

			CONSTEXPR reference at(size_type __n)
			{
				return _AT_Type::_S_ref(_M_elems, __n);
			}

			CONSTEXPR const_reference at(size_type __n) const
			{
				return _AT_Type::_S_ref(_M_elems, __n);
			}

			CONSTEXPR reference front() noexcept
			{
				return *begin();
			}

			CONSTEXPR const_reference front() const noexcept
			{
				return _AT_Type::_S_ref(_M_elems, 0);
			}

			CONSTEXPR reference back() noexcept
			{
				return _Nm ? *(end() - 1) : *end();
			}

			CONSTEXPR const_reference back() const noexcept
			{
				return _Nm ? _AT_Type::_S_ref(_M_elems, _Nm - 1)
				           : _AT_Type::_S_ref(_M_elems, 0);
			}

			CONSTEXPR pointer data() noexcept
			{
				return _AT_Type::_S_ptr(_M_elems);
			}

			CONSTEXPR const_pointer data() const noexcept
			{
				return _AT_Type::_S_ptr(_M_elems);
			}
		};
	}
}
