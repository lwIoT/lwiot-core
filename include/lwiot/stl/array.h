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

		/**
		 * @ingroup stl
		 * @brief Static size array wrapper.
		 * @tparam _Tp Array type.
		 * @tparam _Nm Size of the array (number of elements).
		 */
		template<typename _Tp, size_t _Nm>
		struct Array {
			typedef _Tp value_type; //!< Value type.
			typedef value_type *pointer; //!< Value pointer type.
			typedef const value_type *const_pointer; //!< Value const pointer.
			typedef value_type &reference; //!< Value reference.
			typedef const value_type &const_reference; //!< Value const reference.
			typedef value_type *iterator; //!< Iterator type.
			typedef const value_type *const_iterator; //!< Const iterator type.
			typedef size_t size_type; //!< Size type.
			typedef ptrdiff_t difference_type; //!< Pointer difference type.

		private:
			// Support for zero-sized arrays mandatory.
			typedef lwiot::stl::__array_traits<_Tp, _Nm> _AT_Type;
			typename _AT_Type::_Type _M_elems;

		public:
			/**
			 * @brief Get an iterator to the first element.
			 * @return Iterator pointing to the first element.
			 */
			constexpr iterator begin() noexcept
			{
				return iterator(data());
			}

			/**
			 * @brief Get an iterator to the first element.
			 * @return Iterator pointing to the first element.
			 */
			constexpr const_iterator begin() const noexcept
			{
				return const_iterator(data());
			}

			/**
			 * @brief Pointer past the last element.
			 * @return Iterator pointing to the end of the array.
			 */
			constexpr iterator end() noexcept
			{
				return iterator(data() + _Nm);
			}

			/**
			 * @brief Pointer past the last element.
			 * @return Iterator pointing to the end of the array.
			 */
			constexpr const_iterator end() const noexcept
			{
				return const_iterator(data() + _Nm);
			}

			/**
			 * @brief Get an iterator to the first element.
			 * @return Iterator pointing to the first element.
			 */
			constexpr const_iterator cbegin() const noexcept
			{
				return const_iterator(data());
			}

			/**
			 * @brief Pointer past the last element.
			 * @return Iterator pointing to the end of the array.
			 */
			constexpr const_iterator cend() const noexcept
			{
				return const_iterator(data() + _Nm);
			}

			/**
			 * @brief Get the size of an array.
			 * @return Size of the array.
			 */
			constexpr size_type size() const noexcept
			{
				return _Nm;
			}

			/**
			 * @brief Get the maximum array size.
			 * @return Size of the array.
			 */
			constexpr size_type max_size() const noexcept
			{
				return _Nm;
			}

			/**
			 * @brief Check if \p *this is empty.
			 * @return True or false based on whether or not \p *this is false.
			 */
			constexpr bool empty() const noexcept
			{
				return size() == 0;
			}

			/**
			 * @brief Access operator.
			 * @param __n Element to access.
			 * @return Reference to the element at the \p n th position.
			 */
			constexpr reference operator[](size_type __n) noexcept
			{
				return _AT_Type::_S_ref(_M_elems, __n);
			}

			/**
			 * @brief Access operator.
			 * @param __n Element to access.
			 * @return Reference to the element at the \p n th position.
			 */
			constexpr const_reference operator[](size_type __n) const noexcept
			{
				return _AT_Type::_S_ref(_M_elems, __n);
			}

			/**
			 * @brief Access operator.
			 * @param __n Element to access.
			 * @return Reference to the element at the \p n th position.
			 */
			constexpr reference at(size_type __n)
			{
				return _AT_Type::_S_ref(_M_elems, __n);
			}

			/**
			 * @brief Access operator.
			 * @param __n Element to access.
			 * @return Reference to the element at the \p n th position.
			 */
			constexpr const_reference at(size_type __n) const
			{
				return _AT_Type::_S_ref(_M_elems, __n);
			}

			/**
			 * @brief Access the first element.
			 * @return A reference to the first element.
			 */
			constexpr reference front() noexcept
			{
				return *begin();
			}

			/**
			 * @brief Access the first element.
			 * @return A reference to the first element.
			 */
			constexpr const_reference front() const noexcept
			{
				return _AT_Type::_S_ref(_M_elems, 0);
			}

			/**
			 * @brief Access the last element.
			 * @return A reference to the last element.
			 */
			constexpr reference back() noexcept
			{
				return _Nm ? *(end() - 1) : *end();
			}

			/**
			 * @brief Access the last element.
			 * @return A reference to the last element.
			 */
			constexpr const_reference back() const noexcept
			{
				return _Nm ? _AT_Type::_S_ref(_M_elems, _Nm - 1)
				           : _AT_Type::_S_ref(_M_elems, 0);
			}

			/**
			 * @brief Direct access to the underlying array.
			 * @return The array structure that \p *this wraps around.
			 */
			constexpr pointer data() noexcept
			{
				return _AT_Type::_S_ptr(_M_elems);
			}

			/**
			 * @brief Direct access to the underlying array.
			 * @return The array structure that \p *this wraps around.
			 */
			constexpr const_pointer data() const noexcept
			{
				return _AT_Type::_S_ptr(_M_elems);
			}
		};
	}
}
