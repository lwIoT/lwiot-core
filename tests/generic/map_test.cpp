/*
 * Map unit test.
 *
 * Author: Michel Megens
 * Email: dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/map.h>
#include <lwiot/test.h>

int main(int argc, char **argv)
{
	lwiot_init();

	lwiot::Map<int, double> m1;
	m1.add(5, 1.234);
	m1.add(6, 9.224);
	m1.add(1, 3.234);

	lwiot::Map<int, double> m2(lwiot::stl::move(m1));

	m2[4] = 6.12341;
	m2[6] = 2.1234;

	double six = lwiot::stl::move(m2.at(6));
	print_dbg("Value at 6: %f\n", six);

	assert(m2[6] == 2.1234);
	assert(m2.contains(4));
	assert(m2.contains(1));

	m2.remove(1);
	m2.remove(4);

	assert(!m2.contains(1));
	assert(!m2.contains(4));

	wait_close();
	lwiot_destroy();
	return -EXIT_SUCCESS;
}
