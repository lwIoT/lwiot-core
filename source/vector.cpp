/*
 * Dynamic array implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdint.h>

#include <lwiot/vector.h>

namespace lwiot
{
	void *grow(size_t newsize)
	{
		uint8_t *newptr;

		newptr = (uint8_t*)new uint8_t[newsize];
		return newptr;
	}
}
