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

		inline ~UniquePointer() noexcept
		{
			destroy();
		}

		inline void reset() noexcept
		{
			destroy();
		}

		void reset(T *p) noexcept
		{
			SHARED_ASSERT((nullptr == p) || (px != p));
			destroy();
			px = p;
		}

		void swap(UniquePointer &lhs) noexcept
		{
			lwiot::lib::swap(px, lhs.px);
		}

		inline void release() noexcept
		{
			px = nullptr;
		}

		inline explicit operator bool() const noexcept
		{
			return (nullptr != px);
		}

		inline T &operator*() const noexcept
		{
			SHARED_ASSERT(nullptr != px);
			return *px;
		}

		inline T *operator->() const noexcept
		{
			SHARED_ASSERT(nullptr != px);
			return px;
		}

		inline T *get() const noexcept
		{
			return px;
		}

	private:
		inline void destroy() noexcept
		{
			delete px;
			px = nullptr;
		}

		inline void release() const noexcept
		{
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
