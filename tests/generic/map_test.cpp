/*
 * Map unit test.
 *
 * Author: Michel Megens
 * Email: dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/stl/map.h>
#include <lwiot/test.h>

int main(int argc, char **argv)
{
	lwiot_init();

	lwiot::stl::Map<int, lwiot::String> m1;
	m1.add(5, "Hello");
	m1.add(6, "World");
	m1.add(1, "What?!");

	lwiot::stl::Map<int, lwiot::String> m2(lwiot::stl::move(m1));

	m2[4] = "Bla. Bla";
	m2[6] = "Testing..";
	m2[9] = "More testing..";

	lwiot::String six = lwiot::stl::move(m2.at(6));
	print_dbg("Value at 6: %s\n", six.c_str());

	assert(m2[6].equals("Testing.."));
	assert(m2.contains(4));
	assert(m2.contains(1));

	m2.remove(1);
	m2.remove(4);

	assert(m2.find(4) == m2.end());
	auto iter = m2.find(5);
	assert(iter);
	assert(iter->value.equals("Hello"));

	assert(!m2.contains(1));
	assert(!m2.contains(4));

	print_dbg("\n");
	print_dbg("Remaining entries:\n");
	for(auto& e : m2) {
		print_dbg("Entry: [%i]: %s\n", e.key, e.value.c_str());
	}

	wait_close();
	lwiot_destroy();
	return -EXIT_SUCCESS;
}
