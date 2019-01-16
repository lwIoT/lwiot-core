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
#include <lwiot/sharedpointer.h>

namespace lwiot
{
	SharedPointerCount::SharedPointerCount(lwiot::SharedPointerCount &&count) noexcept : _count(count._count)
	{
		count._count = nullptr;
	}

	SharedPointerCount& SharedPointerCount::operator=(lwiot::SharedPointerCount &&rhs) noexcept
	{
		this->_count = rhs._count;
		rhs._count = nullptr;
		return *this;
	}

	void SharedPointerCount::swap(lwiot::SharedPointerCount &cnt) noexcept
	{
		lwiot::stl::swap(this->_count, cnt._count);
	}

	long SharedPointerCount::useCount() const
	{
		return this->_count->value();
	}

	void SharedPointerCount::acquire() const noexcept
	{
		assert(this->_count != nullptr);
		this->_count->add(1L);
	}
}

