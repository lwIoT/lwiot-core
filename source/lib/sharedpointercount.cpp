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
	SharedPointerCount::SharedPointerCount() : count(nullptr)
	{
	}

	SharedPointerCount::SharedPointerCount(const lwiot::SharedPointerCount &count) : count(count.count)
	{
	}

	void SharedPointerCount::swap(lwiot::SharedPointerCount &count) noexcept
	{
		lwiot::lib::swap(this->count, count.count);
	}

	long SharedPointerCount::useCount() const
	{
		auto rv = 0L;

		if(this->count != nullptr)
			rv = *this->count;

		return rv;
	}
}
