/*
 * Logging unit test.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/log.h>
#include <lwiot/test.h>

int main(int argc, char **argv)
{
	lwiot::Logger logger("log-test");

	logger << "Hello, World! ";
	logger << "Today it is " << 21.1531f << " Degrees!" << lwiot::Logger::newline;
	logger << "End of test";

	wait_close();
	return -EXIT_SUCCESS;
}
