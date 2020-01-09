/*
 * Map unit test.
 *
 * Author: Michel Megens
 * Email: dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/util/ringbuffer.h>
#include <lwiot/log.h>

int main(int argc, char **argv)
{
	lwiot_init();

	lwiot::DefaultRingBuffer buffer;

	print_dbg("Storing in buffer:");

	for(char c = 'a'; c <= 'z'; c++) {
		printf(" %c", c);
		buffer.store(c);
	}

	printf("\n");

	print_dbg("Buffer contents: ");

	while(buffer.available()) {
		printf(" %c", buffer.read());
	}

	printf("\n");

	lwiot_destroy();
	return -EXIT_SUCCESS;
}
