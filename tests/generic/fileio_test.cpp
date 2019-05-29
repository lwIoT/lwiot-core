/*
 * Unit test for File I/O.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <lwiot.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include <lwiot/log.h>
#include <lwiot/types.h>

#include <lwiot/stl/string.h>
#include <lwiot/io/file.h>

#include <lwiot/test.h>

static void write_test(const lwiot::stl::String& path)
{
	lwiot::File file(path, lwiot::FileMode::ReadWrite);
	file.write("Dit is een %s", "test");
}

static void read_test(const lwiot::stl::String& path)
{
	lwiot::stl::String str;

	lwiot::File file(path, lwiot::FileMode::Read);

	str = lwiot::stl::move(file.readString());
	print_dbg("Bytes read from file: %u\n", str.length());
	print_dbg("Read from file: %s\n", str.c_str());
}

int main(int argc, char **argv)
{
	lwiot_init();

	write_test("test.txt");
	read_test("test.txt");

	wait_close();
	lwiot_destroy();

	return -EXIT_SUCCESS;
}
