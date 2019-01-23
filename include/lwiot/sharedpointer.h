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
#include <lwiot/kernel/atomic.h>
#include <lwiot/log.h>

#include <lwiot/stl/move.h>

#ifndef SHARED_ASSERT
#define SHARED_ASSERT(__x__) assert(__x__)
#endif

namespace lwiot
{
	class SharedPointerCount {
	public:
		constexpr SharedPointerCount() : _count(nullptr)
		{ }

		constexpr SharedPointerCount(const SharedPointerCount &count) = default;
		SharedPointerCount(SharedPointerCount&& count) noexcept;
		virtual ~SharedPointerCount() = default;

		SharedPointerCount& operator=(const SharedPointerCount&) = default;
		SharedPointerCount& operator=(SharedPointerCount&& rhs) noexcept;

		void swap(SharedPointerCount &count) noexcept;
		long useCount() const;
		void acquire() const noexcept;

		template<typename T>
		CONSTEXPR void acquire(T *p)
		{
			if(p != nullptr) {
				if(this->_count == nullptr) {
					this->_count = new Atomic<long>(1L);
				} else {
					this->_count->fetch_add(1);
				}
			}
		}

		template<class U>
		CONSTEXPR void release(U *p) noexcept
		{
			if(this->_count != nullptr) {
				this->_count->fetch_sub(1);

				if(this->_count->load() == 0L) {
					if(p)
						delete p;

					delete this->_count;
				}

				this->_count = nullptr;
			}
		}

	private:
		Atomic<long> *_count;
	};

	template<typename T>
	class SharedPointer {
	public:
		typedef T PointerType;

		constexpr explicit SharedPointer() noexcept : _ptr(nullptr), _pn()
		{
		}

		explicit CONSTEXPR SharedPointer(T *p) : _pn()
		{
			this->acquire(p);
		}

		template<class U>
		CONSTEXPR SharedPointer(const SharedPointer<U> &ptr, T *p) : _pn(ptr._pn)
		{
			this->acquire(p);
		}

		template<class U>
		explicit CONSTEXPR SharedPointer(const SharedPointer<U> &ptr) noexcept : _pn(ptr._pn)
		{
			SHARED_ASSERT((NULL == ptr._ptr) || (ptr._pn.useCount() != 0));
			this->acquire(static_cast<typename SharedPointer<T>::PointerType *>(ptr._ptr));
		}

		CONSTEXPR explicit SharedPointer(SharedPointer&& ptr) noexcept : _ptr(ptr._ptr), _pn(stl::move(ptr._pn))
		{
			ptr._ptr = nullptr;
		}

		SharedPointer(const SharedPointer &ptr) noexcept : _pn(ptr._pn)
		{
			this->acquire(ptr._ptr);
		}

		SharedPointer &operator=(const SharedPointer& ptr) noexcept
		{
			if(this->_ptr == ptr._ptr) {
				return *this;
			}

			this->release();
			this->copy(ptr);

			return *this;
		}

		template <typename U>
		inline SharedPointer& operator=(const SharedPointer<U>& other)
		{
			this->release();

			this->_pn = other._pn;
			this->_ptr = static_cast<typename SharedPointer<T>::PointerType *>(other._ptr);
			this->_pn.acquire();

			return *this;
		}

		inline SharedPointer& operator=(SharedPointer&& rhs) noexcept
		{
			if(rhs == *this)
				return *this;

			this->release();
			this->swap(rhs);
			return *this;
		}

		template <class U>
		CONSTEXPR SharedPointer& operator=(SharedPointer<U>&& rhs)
		{
			if(rhs == *this)
				return *this;

			this->release();
			this->swap(rhs);
			return *this;
		}

		inline ~SharedPointer() noexcept
		{
			this->release();
		}

		inline void reset() noexcept
		{
			this->release();
		}

		void reset(T *p)
		{
			SHARED_ASSERT((NULL == p) || (_ptr != p));
			this->release();
			this->acquire(p);
		}

		CONSTEXPR void swap(SharedPointer &lhs) noexcept
		{
			auto tmp = this->_ptr;

			this->_ptr = lhs._ptr;
			lhs._ptr = tmp;
			this->_pn.swap(lhs._pn);
		}

		CONSTEXPR explicit operator bool() const noexcept
		{
			return (this->_pn.useCount() > 0);
		}

		inline bool unique() const noexcept
		{
			return (this->_pn.useCount() == 1);
		}

		inline long useCount() const noexcept
		{
			return this->_pn.useCount();
		}

		CONSTEXPR T& operator*() const noexcept
		{
			assert(this->_ptr != nullptr);
			return *this->_ptr;
		}

		CONSTEXPR T *operator->() const noexcept
		{
			assert(this->_ptr != nullptr);
			return this->_ptr;
		}

		CONSTEXPR T *get() const noexcept
		{
			return this->_ptr;
		}

	private:
		CONSTEXPR void acquire(T *p)
		{
			this->_pn.acquire(p);
			this->_ptr = p;
		}

		CONSTEXPR void release() noexcept
		{
			this->_pn.release(this->_ptr);
			this->_ptr = nullptr;
		}

		inline void copy(const SharedPointer& ptr)
		{
			this->_ptr = ptr._ptr;
			this->_pn = ptr._pn;
			this->_pn.acquire();
		}

		template<class U>
		friend class SharedPointer;

		PointerType *_ptr;
		SharedPointerCount _pn;
	};

	template<class T, class U>
	CONSTEXPR bool operator==(const SharedPointer<T>& l, const SharedPointer<U>& r) noexcept
	{
		return (l.get() == r.get());
	}

	template <typename T>
	CONSTEXPR bool operator==(const SharedPointer<T>& sptr, const T* ptr)
	{
		return sptr.get() == ptr;
	}

	template<class T, class U>
	CONSTEXPR bool operator!=(const SharedPointer<T>& l, const SharedPointer<U>& r) noexcept
	{
		return (l.get() != r.get());
	}

	template<class T, class U>
	CONSTEXPR bool operator<=(const SharedPointer<T>& l, const SharedPointer<U>& r) noexcept
	{
		return (l.get() <= r.get());
	}

	template<class T, class U>
	CONSTEXPR bool operator<(const SharedPointer<T>& l, const SharedPointer<U>& r) noexcept
	{
		return (l.get() < r.get());
	}

	template<class T, class U>
	CONSTEXPR bool operator>=(const SharedPointer<T>& l, const SharedPointer<U>& r) noexcept
	{
		return (l.get() >= r.get());
	}

	template<class T, class U>
	CONSTEXPR bool operator>(const SharedPointer<T>& l, const SharedPointer<U>& r) noexcept
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
