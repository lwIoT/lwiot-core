/*
 * Smart pointer using reference counting.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

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
	template<typename T>
	class UniquePointer {
	public:
		typedef T PointerType;

		constexpr UniquePointer() noexcept : px(nullptr)
		{
		}

		constexpr explicit UniquePointer(T *p) noexcept : px(p)
		{
		}

		template<class U>
		explicit UniquePointer(const UniquePointer<U> &ptr) = delete;
		UniquePointer(const UniquePointer &ptr) = delete;

		UniquePointer(UniquePointer&& ptr) noexcept
		{
			this->px = ptr.release();
		}

		template<class U>
		explicit UniquePointer(UniquePointer<U> &&ptr) noexcept
		{
			this->px = ptr.release();
		}

		UniquePointer &operator=(UniquePointer ptr) noexcept
		{
			this->reset(ptr.release());
			return *this;
		}

		~UniquePointer() noexcept
		{
			destroy();
		}

		void reset() noexcept
		{
			destroy();
		}

		void reset(T *p) noexcept
		{
			assert((p == nullptr) || (px != p));

			this->destroy();
			this->px = p;
		}

		PointerType* release() noexcept
		{
			auto tmp = this->px;
			this->px = nullptr;
			return tmp;
		}

		constexpr explicit operator bool() const noexcept
		{
			return (nullptr != px);
		}

		constexpr T &operator*() const noexcept
		{
			assert(this->px != nullptr);
			return *px;
		}

		constexpr T* operator->() const noexcept
		{
			assert(this->px != nullptr);
			return px;
		}

		constexpr inline T* get() const noexcept
		{
			return px;
		}

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
