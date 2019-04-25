/*
 * Generic unit test for <function.h>.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/log.h>
#include <lwiot/function.h>
#include <lwiot/test.h>

int main(int argc, char **argv)
{
	lwiot::Function<int(int)> fn;

	lwiot_init();
	print_dbg("Testing lambda wrapping..\n");

	fn = [&](int a) -> int {
		return a + 1;
	};

	print_dbg("Fn of 15 = %i\n", fn(15));

	lwiot_destroy();
	wait_close();

	return -EXIT_SUCCESS;
}
