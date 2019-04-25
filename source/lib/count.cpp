/*
 * Integer counting functions.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/function.h>
#include <lwiot/util/count.h>

namespace lwiot
{
	int count(ssize_t start, ssize_t end, Function<int(int)>& functor)
	{
		if(start < end)
			return count_up(start, end, functor);
		else
			return count_down(start, end, functor);
	}

	int count_up(ssize_t start, ssize_t end, Function<int(int)>& functor)
	{
		int rv = 0;

		for(ssize_t i = start; i < end && rv >= 0; i++)
			rv = functor(i);

		return rv;
	}

	int count_down(ssize_t start, ssize_t end, Function<int(int)>& functor)
	{
		int rv = 0;

		for(ssize_t i = start; i >= end && rv >= 0; i--)
			rv = functor(i);

		return rv;
	}
}
