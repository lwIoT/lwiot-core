/*
 * Smart pointer using reference counting.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file sharedpointer.h Shared pointer header.

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
	namespace stl
	{
		namespace detail
		{
			class SharedPointerCount {
			public:
				constexpr SharedPointerCount() : _count(nullptr)
				{
				}

				constexpr SharedPointerCount(const SharedPointerCount &count) = default;

				SharedPointerCount(SharedPointerCount &&count) noexcept;

				virtual ~SharedPointerCount() = default;

				SharedPointerCount &operator=(const SharedPointerCount &) = default;

				SharedPointerCount &operator=(SharedPointerCount &&rhs) noexcept;

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
		}

		/**
		 * @ingroup stl
		 * @brief Smart pointer object that retains owner ship through a pointer.
		 * @tparam T Element type.
		 */
		template<typename T>
		class SharedPointer {
		public:
			typedef T PointerType; //!< Pointer type.
			typedef T element_type; //!< Element type.

			/**
			 * @brief Construct a new shared pointer.
			 */
			constexpr explicit SharedPointer() noexcept : _ptr(nullptr), _pn()
			{
			}

			/**
			 * @brief Construct a new shared pointer.
			 * @param p Initial value.
			 */
			explicit CONSTEXPR SharedPointer(T *p) : _pn()
			{
				this->acquire(p);
			}

			/**
			 * Aliasing constructor.
			 * @tparam U Element type.
			 * @param ptr Shared pointer to copy.
			 * @param p Element value to acquire.
			 */
			template<class U>
			CONSTEXPR SharedPointer(const SharedPointer<U> &ptr, T *p) : _pn(ptr._pn)
			{
				this->acquire(p);
			}

			/**
			 * @brief Copy construct a new shared pointer.
			 * @param ptr Shared pointer to copy.
			 */
			template<class U>
			explicit CONSTEXPR SharedPointer(const SharedPointer<U> &ptr) noexcept : _pn(ptr._pn)
			{
				SHARED_ASSERT((NULL == ptr._ptr) || (ptr._pn.useCount() != 0));
				this->acquire(static_cast<typename SharedPointer<T>::PointerType *>(ptr._ptr));
			}

			/**
			 * @brief Move construct a new shared pointer.
			 * @param ptr Shared pointer to move.
			 */
			CONSTEXPR SharedPointer(SharedPointer &&ptr) noexcept : _ptr(ptr._ptr), _pn(stl::move(ptr._pn))
			{
				ptr._ptr = nullptr;
			}

			/**
			 * @brief Copy construct a new shared pointer.
			 * @param ptr Shared pointer to copy.
			 */
			SharedPointer(const SharedPointer &ptr) noexcept : _pn(ptr._pn)
			{
				this->acquire(ptr._ptr);
			}

			/**
			 * @brief Copy assignment operator.
			 * @param ptr Shared pointer to copy.
			 */
			SharedPointer &operator=(const SharedPointer &ptr) noexcept
			{
				if(this->_ptr == ptr._ptr) {
					return *this;
				}

				this->release();
				this->copy(ptr);

				return *this;
			}

			/**
			 * @brief Copy assignment operator.
			 * @param other Shared pointer to copy.
			 */
			template<typename U>
			inline SharedPointer &operator=(const SharedPointer<U> &other)
			{
				this->release();

				this->_pn = other._pn;
				this->_ptr = static_cast<typename SharedPointer<T>::PointerType *>(other._ptr);
				this->_pn.acquire();

				return *this;
			}

			/**
			 * @brief Move assignment operator.
			 * @param rhs Shared pointer to move.
			 */
			inline SharedPointer &operator=(SharedPointer &&rhs) noexcept
			{
				if(rhs == *this)
					return *this;

				this->release();
				this->swap(rhs);
				return *this;
			}

			/**
			 * @brief Move assignment operator.
			 * @param rhs Shared pointer to move.
			 */
			template<class U>
			CONSTEXPR SharedPointer &operator=(SharedPointer<U> &&rhs)
			{
				if(rhs == *this)
					return *this;

				this->release();
				this->swap(rhs);
				return *this;
			}

			/**
			 * @brief Shared pointer destructor.
			 */
			inline ~SharedPointer() noexcept
			{
				this->release();
			}

			/**
			 * @brief Reset/release ownership.
			 */
			inline void reset() noexcept
			{
				this->release();
			}

			/**
			 * @brief Reset/release ownership of the current object and acquire owner ship over \p p.
			 * @param p Object to acquire.
			 */
			void reset(T *p)
			{
				SHARED_ASSERT((NULL == p) || (_ptr != p));
				this->release();
				this->acquire(p);
			}

			/**
			 * @brief Swap a shared pointer.
			 * @param lhs Shared pointer to swap with \p this.
			 */
			CONSTEXPR void swap(SharedPointer &lhs) noexcept
			{
				auto tmp = this->_ptr;

				this->_ptr = lhs._ptr;
				lhs._ptr = tmp;
				this->_pn.swap(lhs._pn);
			}

			/**
			 * @brief Check the validity of \p *this.
			 * @return True or false based on whether or not \p *this is valid.
			 */
			CONSTEXPR explicit operator bool() const noexcept
			{
				return (this->_pn.useCount() > 0);
			}

			/**
			 * @brief Check the uniqueness of \p *this.
			 * @return True or false based on whether or not \p *this is unique.
			 */
			inline bool unique() const noexcept
			{
				return (this->_pn.useCount() == 1);
			}

			/**
			 * @brief Get the usage count of \p *this.
			 * @return The usage count of \p *this.
			 */
			inline long useCount() const noexcept
			{
				return this->_pn.useCount();
			}

			/**
			 * @brief Access the underlying pointer.
			 * @return A reference to the underlying object.
			 */
			constexpr T& operator*() const noexcept
			{
				assert(this->_ptr != nullptr);
				return *this->_ptr;
			}

			/**
			 * @brief Access the underlying pointer.
			 * @return A reference to the underlying object.
			 */
			CONSTEXPR T* operator->() const noexcept
			{
				assert(this->_ptr != nullptr);
				return this->_ptr;
			}

			/**
			 * @brief Access the underlying pointer.
			 * @return A reference to the underlying object.
			 */
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

			inline void copy(const SharedPointer &ptr)
			{
				this->_ptr = ptr._ptr;
				this->_pn = ptr._pn;
				this->_pn.acquire();
			}

			template<class U>
			friend
			class SharedPointer;

			PointerType *_ptr;
			detail::SharedPointerCount _pn;
		};

		/**
		 * @ingroup stl
		 * @brief Compare two shared pointers.
		 * @param l Shared pointer 1.
		 * @param r Shared pointer 2.
		 * @tparam T Element type of \p l.
		 * @tparam U Element type of \p r.
		 * @return \f$ l \equiv r \f$
		 */
		template<class T, class U>
		CONSTEXPR bool operator==(const SharedPointer<T> &l, const SharedPointer<U> &r) noexcept
		{
			return (l.get() == r.get());
		}

		/**
		 * @ingroup stl
		 * @brief Compare two shared pointers.
		 * @param sptr Shared pointer.
		 * @param ptr Pointer object.
		 * @return \f$ l \equiv r \f$
		 */
		template<typename T>
		CONSTEXPR bool operator==(const SharedPointer<T> &sptr, const T *ptr)
		{
			return sptr.get() == ptr;
		}

		/**
		 * @ingroup stl
		 * @brief Compare two shared pointers.
		 * @param l Shared pointer 1.
		 * @param r Shared pointer 2.
		 * @tparam T Element type of \p l.
		 * @tparam U Element type of \p r.
		 * @return \f$ l \neq r \f$
		 */
		template<class T, class U>
		CONSTEXPR bool operator!=(const SharedPointer<T> &l, const SharedPointer<U> &r) noexcept
		{
			return (l.get() != r.get());
		}

		/**
		 * @ingroup stl
		 * @brief Compare two shared pointers.
		 * @param l Shared pointer 1.
		 * @param r Shared pointer 2.
		 * @tparam T Element type of \p l.
		 * @tparam U Element type of \p r.
		 * @return \f$ l \leq r \f$
		 */
		template<class T, class U>
		CONSTEXPR bool operator<=(const SharedPointer<T> &l, const SharedPointer<U> &r) noexcept
		{
			return (l.get() <= r.get());
		}

		/**
		 * @ingroup stl
		 * @brief Compare two shared pointers.
		 * @param l Shared pointer 1.
		 * @param r Shared pointer 2.
		 * @tparam T Element type of \p l.
		 * @tparam U Element type of \p r.
		 * @return \f$ l < r \f$
		 */
		template<class T, class U>
		CONSTEXPR bool operator<(const SharedPointer<T> &l, const SharedPointer<U> &r) noexcept
		{
			return (l.get() < r.get());
		}

		/**
		 * @ingroup stl
		 * @brief Compare two shared pointers.
		 * @param l Shared pointer 1.
		 * @param r Shared pointer 2.
		 * @tparam T Element type of \p l.
		 * @tparam U Element type of \p r.
		 * @return \f$ l \geq r \f$
		 */
		template<class T, class U>
		CONSTEXPR bool operator>=(const SharedPointer<T> &l, const SharedPointer<U> &r) noexcept
		{
			return (l.get() >= r.get());
		}

		/**
		 * @ingroup stl
		 * @brief Compare two shared pointers.
		 * @param l Shared pointer 1.
		 * @param r Shared pointer 2.
		 * @tparam T Element type of \p l.
		 * @tparam U Element type of \p r.
		 * @return \f$ l > r \f$
		 */
		template<class T, class U>
		CONSTEXPR bool operator>(const SharedPointer<T> &l, const SharedPointer<U> &r) noexcept
		{
			return (l.get() > r.get());
		}

		/**
		 * @ingroup stl
		 * @brief Cast a shared pointer.
		 * @brief Shared pointer to cast.
		 * @tparam T Element type of \p ptr.
		 * @tparam U Element type of the return value.
		 * @return Shared pointer with element type \p U.
		 */
		template<class T, class U>
		SharedPointer<T> static_pointer_cast(const SharedPointer<U> &ptr)
		{
			return SharedPointer<T>(ptr, static_cast<typename SharedPointer<T>::PointerType *>(ptr.get()));
		}

		/**
		 * @ingroup stl
		 * @brief Cast a shared pointer.
		 * @brief Shared pointer to cast.
		 * @tparam T Element type of \p ptr.
		 * @tparam U Element type of the return value.
		 * @return Shared pointer with element type \p U.
		 */
		template<class T, class U>
		SharedPointer<T> dynamic_pointer_cast(const SharedPointer<U> &ptr)
		{
			T *p = dynamic_cast<typename SharedPointer<T>::PointerType *>(ptr.get());
			if(p != nullptr) {
				return SharedPointer<T>(ptr, p);
			} else {
				return SharedPointer<T>();
			}
		}

		/**
		 * @ingroup stl
		 * @brief Allocate and construct an object of type T passing \p args to the constructor.
		 * @tparam T Object type.
		 * @tparam Args Argument types.
		 * @param args Arguments to pass to the constructor of \p T.
		 * @return A shared pointer object of type \p T.
		 */
		template<typename T, typename... Args>
		SharedPointer<T> MakeShared(Args &&... args)
		{
			return SharedPointer<T>(new T(stl::forward<Args>(args)...));
		}
	}
}
