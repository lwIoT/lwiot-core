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

		UniquePointer(void) throw() :
				px(NULL)
		{
		}

		explicit UniquePointer(T *p) throw() : px(p)
		{
		}

		template<class U>
		UniquePointer(const UniquePointer<U> &ptr) throw() : px(
				static_cast<typename UniquePointer<T>::PointerType *>(ptr.px))
		{
			const_cast<UniquePointer<U> &>(ptr).px = NULL;
		}

		UniquePointer(const UniquePointer &ptr) throw() :
				px(ptr.px)
		{
			const_cast<UniquePointer &>(ptr).px = NULL;
		}

		UniquePointer &operator=(UniquePointer ptr) throw()
		{
			swap(ptr);
			return *this;
		}

		inline ~UniquePointer(void) throw()
		{
			destroy();
		}

		inline void reset(void) throw()
		{
			destroy();
		}

		void reset(T *p) throw()
		{
			SHARED_ASSERT((NULL == p) || (px != p));
			destroy();
			px = p;
		}

		void swap(UniquePointer &lhs) throw()
		{
			lwiot::lib::swap(px, lhs.px);
		}

		inline void release(void) throw()
		{
			px = NULL;
		}

		inline operator bool() const throw()
		{
			return (nullptr != px);
		}

		inline T &operator*() const throw()
		{
			SHARED_ASSERT(NULL != px);
			return *px;
		}

		inline T *operator->() const throw()
		{
			SHARED_ASSERT(NULL != px);
			return px;
		}

		inline T *get(void) const throw()
		{
			return px;
		}

	private:
		inline void destroy(void) throw()
		{
			delete px;
			px = NULL;
		}

		inline void release(void) const throw()
		{
			px = NULL;
		}

	private:
		T *px;
	};

	template<class T, class U>
	inline bool operator==(const UniquePointer <T> &l, const UniquePointer <U> &r) throw()
	{
		return (l.get() == r.get());
	}

	template<class T, class U>
	inline bool operator!=(const UniquePointer <T> &l, const UniquePointer <U> &r) throw()
	{
		return (l.get() != r.get());
	}

	template<class T, class U>
	inline bool operator<=(const UniquePointer <T> &l, const UniquePointer <U> &r) throw()
	{
		return (l.get() <= r.get());
	}

	template<class T, class U>
	inline bool operator<(const UniquePointer <T> &l, const UniquePointer <U> &r) throw()
	{
		return (l.get() < r.get());
	}

	template<class T, class U>
	inline bool operator>=(const UniquePointer <T> &l, const UniquePointer <U> &r) throw()
	{
		return (l.get() >= r.get());
	}

	template<class T, class U>
	inline bool operator>(const UniquePointer <T> &l, const UniquePointer <U> &r) throw()
	{
		return (l.get() > r.get());
	}
}
