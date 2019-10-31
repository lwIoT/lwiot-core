/*
 * Map unit test.
 *
 * Author: Michel Megens
 * Email: dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/sharedpointer.h>
#include <lwiot/test.h>

int main(int argc, char **argv)
{
	lwiot_init();
	int *ptr = new int;
	int *newptr = new int;


	*ptr = 4;
	*newptr = 15;

	lwiot::SharedPointer<int> s0(newptr);
	lwiot::SharedPointer<int> s1(ptr);
	lwiot::SharedPointer<int> s2(s1);
	lwiot::SharedPointer<int> s3(s2);
	auto s5 = lwiot::stl::MakeShared<int>(4);

	*s2 = 10;

	print_dbg("Shared pointer value: %i\n", *s3);
	lwiot::SharedPointer<int> s4(lwiot::stl::move(s1));
	++*s2;

	s4 = lwiot::stl::move(s0);
	s0 = s2;
	print_dbg("Shared pointer 4 value: %i\n", *s4);
	print_dbg("Shared pointer 0 value: %i\n", *s0);
	print_dbg("Shared pointer 5 value: %i\n", *s5);

	assert(s4 == newptr);
	assert(s4);

	wait_close();
	lwiot_destroy();
	return -EXIT_SUCCESS;
}
