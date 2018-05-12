/*
 * Vector unit test.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdint.h>
#include <lwiot.h>
#include <assert.h>

#include <lwiot/log.h>
#include <lwiot/test.h>
#include <lwiot/vector.h>

int main(int argc, char**argv)
{
	lwiot::Vector<int> vec;

	lwiot_init();
	UNUSED(argc);
	UNUSED(argv);

	assert(vec.length() == 0);
	vec.add(2);
	vec.add(21);
	vec.add(120);
	vec[3] = 510;

	for(auto entry : vec) {
		print_dbg("Entry: %i\n", entry);
	}

	assert(vec[1] == 21);
	assert(vec[0] == 2);
	assert(vec[3] == 510);

	lwiot_destroy();
	wait_close();
	return -EXIT_SUCCESS;
}
