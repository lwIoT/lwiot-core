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
#include <lwiot/log.h>

#include <lwiot/stl/string.h>
#include <lwiot/stl/skiplist.h>
#include <lwiot/stl/referencewrapper.h>
#include <lwiot/stl/move.h>

static void dumplist()
{
	lwiot::stl::SkipList<int, lwiot::String> sl1;

	sl1.insert(1, "ABC");
	sl1.insert(2, "ABC");
	sl1.insert(3, "ABC");
	sl1.insert(4, "ABC");
	sl1.insert(5, "ABC");
	sl1.insert(6, "ABC");
	sl1.insert(7, "ABC");
	sl1.insert(8, "ABC");
	sl1.insert(9, "ABC");
	sl1.insert(0, "ABC");

	sl1.insert(10, "ABC");
	sl1.insert(20, "ABC");
	sl1.insert(30, "ABC");
	sl1.insert(40, "ABC");
	sl1.insert(50, "ABC");
	sl1.insert(60, "ABC");
	sl1.insert(70, "ABC");
	sl1.insert(80, "ABC");
	sl1.insert(90, "ABC");
	sl1.insert(100, "ABC");

	auto rv = sl1.emplace(9, "BDA");

	print_dbg("Emplaced: %s\n", rv.second ? "true" : "false");
	print_dbg("Value at 9: %s\n", sl1[9].c_str());

	assert(sl1.size() == 20);
	sl1.dump("Level %i\n\t", "%i ");
}

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
	const auto& v1 = sl2.at(7);

	assert(v1 == "DEF");
	const auto& v2 = sl1.at(4);
	assert(v2 == "XYZ");

	print_dbg("Insert test done!\n");
}

int main(int argc, char **argv)
{
	lwiot_init();
	skiplist_test();
	dumplist();

	wait_close();
	lwiot_destroy();

	return -EXIT_SUCCESS;
}
