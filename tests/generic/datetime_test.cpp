/*
 * C#-style DateTime implementation.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <lwiot.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include <lwiot/log.h>
#include <lwiot/stl/string.h>
#include <lwiot/types.h>
#include <lwiot/util/datetime.h>
#include <lwiot/test.h>

static void dt_test()
{
	lwiot::DateTime dt;
	lwiot::DateTime dt2(31556926);
	lwiot::DateTime result(0);

	result = dt + dt2;
	print_dbg("Time: %s\n", lwiot::DateTime::now().toString().c_str());

	assert(dt.year() + 1 == result.year());
	assert(result > dt && dt < result);
}

int main(int argc, char **argv)
{
	lwiot_init();
	dt_test();
	wait_close();
	lwiot_destroy();

	return -EXIT_SUCCESS;
}
