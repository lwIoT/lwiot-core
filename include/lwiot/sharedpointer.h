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
#include <lwiot/atomic.h>
#include <lwiot/log.h>

#ifndef SHARED_ASSERT
#define SHARED_ASSERT(__x__) assert(__x__)
#endif

namespace lwiot
{
	class SharedPointerCount {
	public:
		SharedPointerCount();
		SharedPointerCount(const SharedPointerCount &count);

		void swap(SharedPointerCount &count) noexcept;
		long useCount() const;

		template<typename T>
		void acquire(T *p)
		{
			if(p != nullptr) {
				if(this->count == nullptr) {
					this->count = new Atomic<long>(1L);
				} else {
					this->count->add(1);
				}
			}
		}

		template<class U>
		void release(U *p) noexcept
		{
			if(this->count != nullptr) {
				this->count->substract(1);

				if(this->count->value() == 0L) {
					if(p)
						delete p;

					delete this->count;
				}

				this->count = nullptr;
			}
		}

	private:
		Atomic<long> *count;
	};

	template<typename T>
	class SharedPointer {
	public:
		typedef T PointerType;

		SharedPointer() noexcept : ptr(nullptr), pn()
		{
		}

		explicit SharedPointer(T *p) : pn()
		{
			acquire(p);
		}

		template<class U>
		SharedPointer(const SharedPointer<U> &ptr, T *p) : pn(ptr.pn)
		{
			acquire(p);
		}

		template<class U>
		explicit SharedPointer(const SharedPointer<U> &ptr) noexcept : pn(ptr.pn)
		{
			SHARED_ASSERT((NULL == ptr.ptr) || (0 != ptr.pn.useCount()));

			acquire(static_cast<typename SharedPointer<T>::PointerType *>(ptr.ptr));
		}

		SharedPointer(const SharedPointer &ptr) noexcept : pn(ptr.pn)
		{
			acquire(ptr.ptr);
		}

		SharedPointer &operator=(SharedPointer pointer) noexcept
		{
			if(pointer == *this) {
				return *this;
			}

			release();
			swap(pointer);

			return *this;
		}

		template <typename U>
		SharedPointer& operator=(SharedPointer<U> other)
		{
			release();
			swap(other);

			return *this;
		}

		inline ~SharedPointer() noexcept
		{
			release();
		}

		inline void reset() noexcept
		{
			release();
		}

		void reset(T *p)
		{
			SHARED_ASSERT((NULL == p) || (ptr != p));
			release();
			acquire(p);
		}

		void swap(SharedPointer &lhs) noexcept
		{
			lwiot::lib::swap(this->ptr, lhs.ptr);
			pn.swap(lhs.pn);
		}

		inline explicit operator bool() const noexcept
		{
			return (0 < pn.useCount());
		}

		inline bool unique() const noexcept
		{
			return (1 == pn.useCount());
		}

		long useCount() const noexcept
		{
			return pn.useCount();
		}

		inline T &operator*() const noexcept
		{
			SHARED_ASSERT(NULL != ptr);
			return *ptr;
		}

		inline T *operator->() const noexcept
		{
			SHARED_ASSERT(NULL != ptr);
			return ptr;
		}

		inline T *get() const noexcept
		{
			return ptr;
		}

	private:
		inline void acquire(T *p)
		{
			pn.acquire(p);
			ptr = p;
		}

		inline void release() noexcept
		{
			pn.release(ptr);
			ptr = NULL;
		}

		template<class U>
		friend
		class SharedPointer;

		T *ptr;
		SharedPointerCount pn;
	};

	template<class T, class U> inline bool operator==(const SharedPointer<T>& l, const SharedPointer<U>& r) noexcept
	{
		return (l.get() == r.get());
	}
	template<class T, class U> inline bool operator!=(const SharedPointer<T>& l, const SharedPointer<U>& r) noexcept
	{
		return (l.get() != r.get());
	}
	template<class T, class U> inline bool operator<=(const SharedPointer<T>& l, const SharedPointer<U>& r) noexcept
	{
		return (l.get() <= r.get());
	}
	template<class T, class U> inline bool operator<(const SharedPointer<T>& l, const SharedPointer<U>& r) noexcept
	{
		return (l.get() < r.get());
	}
	template<class T, class U> inline bool operator>=(const SharedPointer<T>& l, const SharedPointer<U>& r) noexcept
	{
		return (l.get() >= r.get());
	}
	template<class T, class U> inline bool operator>(const SharedPointer<T>& l, const SharedPointer<U>& r) noexcept
	{
		return (l.get() > r.get());
	}

	template<class T, class U>
	SharedPointer<T> static_pointer_cast(const SharedPointer<U>& ptr)
	{
		return SharedPointer<T>(ptr, static_cast<typename SharedPointer<T>::element_type*>(ptr.get()));
	}

	template<class T, class U>
	SharedPointer<T> dynamic_pointer_cast(const SharedPointer<U>& ptr)
	{
		T* p = dynamic_cast<typename SharedPointer<T>::element_type*>(ptr.get());
		if (p != nullptr)
		{
			return SharedPointer<T>(ptr, p);
		}
		else
		{
			return SharedPointer<T>();
		}
	}
}
