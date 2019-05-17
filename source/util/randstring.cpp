/*
 * Generate a random string.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/stl/string.h>

namespace lwiot
{
	static void __randstr(lwiot::stl::String& str, size_t size)
	{
		const char charset[] = "abcdefghijklmnopqrstuvwxyz";

		if (size) {
			--size;
			for (size_t n = 0; n < size; n++) {
				int key = rand() % (int) (sizeof charset - 1);
				str += charset[key];
			}
		}
	}

	stl::String randstr(const stl::String& prefix, size_t size)
	{
		stl::String copy(prefix);

		__randstr(copy, size);
		return copy;
	}
}


