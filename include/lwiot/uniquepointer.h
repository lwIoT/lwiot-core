/*
 * Smart pointer using reference counting.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file uniquepointer.h

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/log.h>

#include <lwiot/stl/move.h>

#ifndef assert
#define assert(__x__) assert(__x__)
#endif

namespace lwiot
{
	/**
	 * @brief RAII smart pointer.
	 * @tparam T Pointer type.
	 * @ingroup stl
	 */
	template<typename T>
	class UniquePointer {
	public:
		typedef T PointerType; //!< Pointer type.

		/**
		 * @brief Construct a new smart pointer object.
		 */
		constexpr UniquePointer() noexcept : px(nullptr)
		{
		}

		/**
		 * @brief Construct a new smart pointer object.
		 * @param p Pointer object.
		 */
		constexpr explicit UniquePointer(T *p) noexcept : px(p)
		{
		}

		template<class U>
		explicit UniquePointer(const UniquePointer<U> &ptr) = delete;
		UniquePointer(const UniquePointer &ptr) = delete;

		/**
		 * @brief Construct a new smart pointer object.
		 * @param ptr Pointer object.
		 */
		UniquePointer(UniquePointer&& ptr) noexcept
		{
			this->px = ptr.release();
		}

		/**
		 * @brief Construct a new smart pointer object.
		 * @param ptr Pointer object.
		 */
		template<class U>
		explicit UniquePointer(UniquePointer<U> &&ptr) noexcept
		{
			this->px = ptr.release();
		}

		/**
		 * @brief Construct a new smart pointer object.
		 * @param ptr Pointer object.
		 */
		UniquePointer &operator=(UniquePointer ptr) noexcept
		{
			this->reset(ptr.release());
			return *this;
		}

		/**
		 * @brief Destroy a UniquePointer object and deallocate the underlying pointer.
		 */
		~UniquePointer() noexcept
		{
			destroy();
		}

		/**
		 * @brief Deallocate the underlying pointer and reset the UniquePointer.
		 */
		void reset() noexcept
		{
			destroy();
		}

		/**
		 * @brief Reset a UniquePointer
		 * @param p New pointer object.
		 */
		void reset(T *p) noexcept
		{
			assert((p == nullptr) || (px != p));

			this->destroy();
			this->px = p;
		}

		/**
		 * @brief Release control of the underlying pointer.
		 * @return The underlying pointer object.
		 */
		PointerType* release() noexcept
		{
			auto tmp = this->px;
			this->px = nullptr;
			return tmp;
		}

		/**
		 * @brief Check the validity of \p *this.
		 * @return A success indicator.
		 */
		constexpr explicit operator bool() const noexcept
		{
			return (nullptr != px);
		}

		/**
		 * @brief Access the underlying pointer.
		 * @return A reference to the underlying object.
		 */
		constexpr T &operator*() const noexcept
		{
			assert(this->px != nullptr);
			return *px;
		}

		/**
		 * @brief Access the underlying pointer.
		 * @return The underlying pointer.
		 */
		constexpr T* operator->() const noexcept
		{
			assert(this->px != nullptr);
			return px;
		}

		/**
		 * @brief Access the underlying pointer.
		 * @return The underlying pointer.
		 */
		constexpr inline T* get() const noexcept
		{
			return px;
		}

		/**
		 * @brief Swap two UniquePointer objects.
		 * @param a The first pointer object.
		 * @param b The second pointer object.
		 * @ingroup stl
		 */
		friend void swap(UniquePointer& a, UniquePointer& b)
		{
			using stl::swap;
			swap(a.px, b.px);
		}

	private:
		void destroy() noexcept
		{
			if(px == nullptr)
				return;

			delete px;
			px = nullptr;
		}

	private:
		PointerType* px;
	};

	namespace detail
	{
		template<class _Ty> struct _Unique_if {
			typedef UniquePointer<_Ty> _Single_object;
		};

		template<class _Ty> struct _Unique_if<_Ty[]> {
			typedef UniquePointer<_Ty[]> _Unknown_bound;
		};

		template<class _Ty, size_t N> struct _Unique_if<_Ty[N]> {
			typedef void _Known_bound;
		};
	}

	/**
	 * @ingroup stl
	 * @brief Allocate and construct an object of type T passing \p args to the constructor.
	 * @tparam T Object type.
	 * @tparam Args Argument types.
	 * @param args Arguments to pass to the constructor of \p T.
	 * @return A UniquePointer object of type \p T.
	 */
	template <typename T, typename... Args>
	typename detail::_Unique_if<T>::_Single_object MakeUnique(Args&&... args)
	{
		return UniquePointer<T>(new T(stl::forward<Args>(args)...));
	}

	template<class T, class U>
	inline bool operator==(const UniquePointer <T> &l, const UniquePointer <U> &r) noexcept
	{
		return (l.get() == r.get());
	}

	template<class T, class U>
	inline bool operator!=(const UniquePointer <T> &l, const UniquePointer <U> &r) noexcept
	{
		return (l.get() != r.get());
	}

	template<class T, class U>
	inline bool operator<=(const UniquePointer <T> &l, const UniquePointer <U> &r) noexcept
	{
		return (l.get() <= r.get());
	}

	template<class T, class U>
	inline bool operator<(const UniquePointer <T> &l, const UniquePointer <U> &r) noexcept
	{
		return (l.get() < r.get());
	}

	template<class T, class U>
	inline bool operator>=(const UniquePointer <T> &l, const UniquePointer <U> &r) noexcept
	{
		return (l.get() >= r.get());
	}

	template<class T, class U>
	inline bool operator>(const UniquePointer <T> &l, const UniquePointer <U> &r) noexcept
	{
		return (l.get() > r.get());
	}
}
