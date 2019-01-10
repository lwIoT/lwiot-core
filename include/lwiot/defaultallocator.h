/*
 * Default C++ allocator implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot.h>

#ifdef CXX
#include <stddef.h>

namespace lwiot
{
	template <typename T>
	struct DefaultAllocator {
	public:
		typedef T ObjectType;

		explicit CONSTEXPR DefaultAllocator() = default;
		virtual ~DefaultAllocator() = default;

		ObjectType* allocate(size_t bytes) const noexcept
		{
			auto data = lwiot_mem_alloc(bytes * sizeof(ObjectType));
			return reinterpret_cast<ObjectType*>(data);
		}

		void deallocate(ObjectType* obj, size_t bytes) const noexcept
		{
			if(obj == nullptr || bytes == 0UL)
				return;

			lwiot_mem_free(obj);
		}

		CONSTEXPR void construct(ObjectType *obj, const ObjectType& t) const
		{
			new(obj) ObjectType(t);
		}

		CONSTEXPR void destroy(ObjectType* obj) const
		{
			obj->~ObjectType();
		}
	};
}
#endif
