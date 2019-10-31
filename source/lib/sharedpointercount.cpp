/*
 * Smart pointer using reference counting.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/log.h>

#include <lwiot/stl/sharedpointer.h>

namespace lwiot
{
	namespace stl
	{
		namespace detail
		{
			SharedPointerCount::SharedPointerCount(SharedPointerCount &&count) noexcept : _count(
					count._count)
			{
				count._count = nullptr;
			}

			SharedPointerCount &SharedPointerCount::operator=(SharedPointerCount &&rhs) noexcept
			{
				this->_count = rhs._count;
				rhs._count = nullptr;
				return *this;
			}

			void SharedPointerCount::swap(SharedPointerCount &cnt) noexcept
			{
				lwiot::stl::swap(this->_count, cnt._count);
			}

			long SharedPointerCount::useCount() const
			{
				if(this->_count == nullptr)
					return 0L;

				return this->_count->load();
			}

			void SharedPointerCount::acquire() const noexcept
			{
				assert(this->_count != nullptr);
				this->_count->fetch_add(1L);
			}
		}
	}
}

