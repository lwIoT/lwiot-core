/*
 * GUID unit test.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/log.h>
#include <lwiot/util/guid.h>
#include <lwiot/test.h>

#include <lwiot/stl/move.h>

int main(int argc, char **argv)
{
	lwiot_init();
	print_dbg("Test GUID implementation!\n");

	lwiot::Guid g1;
	lwiot::Guid g2;

	lwiot::Guid g3(g1);
	lwiot::Guid g4(g2);

	assert(g1 == g3);
	assert(g2 == g4);

	lwiot::stl::swap(g1, g2);
	assert(g2 == g3);
	assert(g1 == g4);
	lwiot::stl::swap(g1, g2);

	print_dbg("Guid g1: %s\n", g1.toString().c_str());
	print_dbg("Guid g2: %s\n", g2.toString().c_str());
	assert(g1 != g2);

	lwiot::Guid g5 = lwiot::stl::move(g2);
	print_dbg("Guid g5: %s\n", g5.toString().c_str());
	assert(g2 == g5);

	lwiot_destroy();
	wait_close();

	return -EXIT_SUCCESS;
}
