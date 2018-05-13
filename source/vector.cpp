/*
 * Dynamic array implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <lwiot/vector.h>

namespace lwiot
{
	void *grow(uint8_t *old, size_t oldsize)
	{
		uint8_t *newptr;
		size_t newsize;

		newsize = oldsize * 2U;
		newptr = (uint8_t*)new uint8_t[newsize];
		memcpy(newptr, old, oldsize);

		delete [] old;
		return newptr;
	}
}
