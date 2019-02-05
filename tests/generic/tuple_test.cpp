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
#include <float.h>
#include <string.h>

#include <lwiot/test.h>
#include <lwiot/stl/tuple.h>
#include <lwiot/stl/string.h>

constexpr bool Equals(float a, float b)
{
	auto diff = a - b;
	return (diff < FLT_EPSILON) && (-diff < FLT_EPSILON);
}

int main(int argc, char **argv)
{
	lwiot_init();
	auto tuple = lwiot::stl::MakeTuple(1, 211.9f, 234, lwiot::stl::String("ABC"));

	assert(lwiot::stl::get<0>(tuple) == 1);
	assert(Equals(lwiot::stl::get<1>(tuple), 211.9f));
	assert(lwiot::stl::get<2>(tuple) == 234);
	assert(lwiot::stl::get<3>(tuple) == "ABC");
	assert(Equals(lwiot::stl::get<float>(tuple), 211.9f));

	lwiot_destroy();
	wait_close();
	return -EXIT_SUCCESS;
}
