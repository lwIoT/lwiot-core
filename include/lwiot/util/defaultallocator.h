/*
 * Default C++ allocator implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file defaultallocator.h

#pragma once

#include <lwiot.h>

#ifdef CXX

#include <lwiot/stl/move.h>
#include <lwiot/stl/forward.h>

#include <stddef.h>

namespace lwiot
{
	/**
	 * @ingroup util
	 * @brief Allocator implementation.
	 * @tparam T Allocation type.
	 */
	template <typename T>
	struct DefaultAllocator {
	public:
		typedef T ObjectType; //!< Object type.

		/**
		 * @brief Create a new allocator object.
		 */
		explicit CONSTEXPR DefaultAllocator() = default;

		/**
		 * @brief Destroy an allocator.
		 */
		virtual ~DefaultAllocator() = default;

		/**
		 * @brief Allocate a number of objects of type ObjectType.
		 * @param num Number of objects to allocate.
		 * @return A memory address.
		 */
		ObjectType* allocate(size_t num) const noexcept
		{
			auto data = lwiot_mem_alloc(num * sizeof(ObjectType));
			return reinterpret_cast<ObjectType*>(data);
		}

		/**
		 * @brief Deallocate a memory region.
		 * @param obj Memory region to allocate.
		 * @param bytes Ignored.
		 */
		void deallocate(ObjectType* obj, size_t bytes) const noexcept
		{
			if(obj == nullptr || bytes == 0UL)
				return;

			lwiot_mem_free(obj);
		}

		/**
		 * @brief Move construct an object.
		 * @param obj Memory to move into.
		 * @param t Object to move.
		 */
		void move(ObjectType *obj, ObjectType& t) const
		{
			new(obj) ObjectType(stl::move(t));
		}

		/**
		 * @brief Construct a new object.
		 * @param obj Object to construct.
		 * @param t Object to copy into \p obj.
		 */
		void construct(ObjectType *obj, const ObjectType& t) const
		{
			new(obj) ObjectType(t);
		}

		/**
		 * @brief Destroy an object.
		 * @param obj Object to destroy.
		 */
		void destroy(ObjectType* obj) const noexcept
		{
			obj->~ObjectType();
		}
	};
}
#endif
