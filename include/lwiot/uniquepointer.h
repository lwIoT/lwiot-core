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

#ifndef SHARED_ASSERT
#define SHARED_ASSERT(__x__) assert(__x__)
#endif

namespace lwiot
{
	template<typename T>
	class UniquePointer {
	public:
		using PointerType = T;

		UniquePointer() noexcept : px(nullptr)
		{
		}

		explicit UniquePointer(T *p) noexcept : px(p)
		{
		}

		template<class U>
		explicit UniquePointer(const UniquePointer<U> &ptr) noexcept : px(
				static_cast<typename UniquePointer<T>::PointerType *>(ptr.px))
		{
			const_cast<UniquePointer<U> &>(ptr).px = nullptr;
		}

		UniquePointer(const UniquePointer &ptr) noexcept :
				px(ptr.px)
		{
			const_cast<UniquePointer &>(ptr).px = nullptr;
		}

		UniquePointer(UniquePointer&& ptr) noexcept : px(ptr.px)
		{
			ptr.px = nullptr;
		}

		template<class U>
		explicit UniquePointer(UniquePointer<U> &&ptr) noexcept
		{
			this->px = ptr.px;
			ptr.px = nullptr;
		}

		UniquePointer &operator=(UniquePointer ptr) noexcept
		{
			swap(ptr);
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
			SHARED_ASSERT((nullptr == p) || (px != p));
			destroy();
			px = p;
		}

		constexpr void swap(UniquePointer &lhs) noexcept
		{
			lwiot::stl::swap(px, lhs.px);
		}

		constexpr void release() noexcept
		{
			px = nullptr;
		}

		constexpr explicit operator bool() const noexcept
		{
			return (nullptr != px);
		}

		constexpr T &operator*() const noexcept
		{
			SHARED_ASSERT(this->px != nullptr);
			return *px;
		}

		constexpr T* operator->() const noexcept
		{
			SHARED_ASSERT(this->px != nullptr);
			return px;
		}

		constexpr inline T* get() const noexcept
		{
			return px;
		}

	private:
		inline void destroy() noexcept
		{
			delete px;
			px = nullptr;
		}

	private:
		T *px;
	};

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
