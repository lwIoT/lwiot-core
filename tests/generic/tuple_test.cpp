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
#include <string.h>

#include <lwiot/test.h>
#include <lwiot/stl/tuple.h>

int main(int argc, char **argv)
{
	lwiot_init();
	auto tuple = lwiot::stl::MakeTuple(1, 211.9f, "ABC");

	assert(lwiot::stl::get<0>(tuple) == 1);
	assert(lwiot::stl::get<1>(tuple) == 211.9f);
	assert(strcmp(lwiot::stl::get<2>(tuple), "ABC") == 0);

	lwiot_destroy();
	wait_close();
	return -EXIT_SUCCESS;
}
