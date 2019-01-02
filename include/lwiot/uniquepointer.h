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

		UniquePointer(UniquePointer&& ptr) noexcept : px(ptr.px)
		{
			ptr.release();
		}

		template<class U>
		explicit UniquePointer(UniquePointer<U> &&ptr) noexcept : px(ptr.px)
		{
			ptr.release();
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
			assert((nullptr == p) || (px != p));
			destroy();
			px = p;
		}

		PointerType* release() noexcept
		{
			auto tmp = this->px;
			px = nullptr;
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

	private:
		void destroy() noexcept
		{
			delete px;
			px = nullptr;
		}

	private:
		PointerType* px;
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
