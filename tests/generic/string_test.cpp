/*
 * String unit test.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>
#include <assert.h>

#include <lwiot/string.h>
#include <lwiot/log.h>
#include <lwiot/test.h>

#define FOOBAR_STR "The usual: Foo Bar"
#define HELLOWORLD_STR "The usual: Hello, World!"

int main(int argc, char **argv)
{
	lwiot_init();

	lwiot::String s1("Foo Bar");
	lwiot::String s2 = "Hello, World!";
	lwiot::String s3;

	s3 += "The usual: ";
	lwiot::String s4(s3);
	s3 += s2;

	lwiot::String s5 = s4 + s1;

	print_dbg("%s\n", s5.c_str());
	print_dbg("%s\n", s3.c_str());

	assert(strcmp(s5.c_str(), FOOBAR_STR) == 0);
	assert(strcmp(s3.c_str(), HELLOWORLD_STR) == 0);

	wait_close();
	return -EXIT_SUCCESS;
}
