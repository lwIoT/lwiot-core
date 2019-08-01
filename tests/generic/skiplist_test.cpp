/*
 * Skip list list unit test.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/test.h>

#include <lwiot/stl/string.h>
#include <lwiot/stl/skiplist.h>
#include <lwiot/stl/referencewrapper.h>
#include <lwiot/stl/move.h>
#include <lwiot/stl/foreach.h>

static void skiplist_test()
{
	lwiot::stl::SkipList<int, lwiot::String> sl1;
	lwiot::stl::SkipList<int, lwiot::String> sl2;
	lwiot::stl::String bac("BAC");

	sl1.insert(4, "ABC");
	sl1.insert(7, "DEF");

	sl2 = lwiot::stl::move(sl1);
	sl1 = sl2;

	sl1[4] = "XYZ";
	sl1[-1] = bac;

	sl2.erase(4);

	print_dbg("Insert test done!");
}

int main(int argc, char **argv)
{
	lwiot_init();
	skiplist_test();

	wait_close();
	lwiot_destroy();

	return -EXIT_SUCCESS;
}
