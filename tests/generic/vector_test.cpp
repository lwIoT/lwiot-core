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
#include <lwiot/sharedpointer.h>

static bool shared_ptr_test(void *expected, lwiot::SharedPointer<lwiot::Vector<int>> vec)
{
	return vec.get() == expected;
}

int main(int argc, char**argv)
{
	lwiot::SharedPointer<lwiot::Vector<int>> vec(new lwiot::Vector<int>());
	lwiot::SharedPointer<lwiot::Vector<int>> ptr;
	lwiot_init();

	UNUSED(argc);
	UNUSED(argv);

	vec->add(2);
	vec->add(21);
	vec->add(120);
	(*vec)[3] = 510;

	ptr = vec;

	for(auto entry : *vec) {
		print_dbg("Entry: %i\n", entry);
	}

	assert((*vec)[1] == 21);
	assert((*vec)[0] == 2);
	assert((*vec)[3] == 510);
	assert(shared_ptr_test(vec.get(), ptr));

	lwiot_destroy();
	wait_close();
	return -EXIT_SUCCESS;
}
