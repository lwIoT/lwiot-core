/*
 * Buffered stream unit test.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>
#include <assert.h>

#include <lwiot/log.h>
#include <lwiot/bufferedstream.h>
#include <lwiot/test.h>

int main(int argc, char **argv)
{
	UNUSED(argc);
	UNUSED(argv);

	lwiot_init();
	print_dbg("Creating buffered stream..\n");
	lwiot::BufferedStream bs(16);

	assert(bs.size() == 16);

	bs << "Hello, World!";
	bs << " - Extending the string to trigger the buffer grow..";
	assert(bs.size() == 81);
	assert(bs.available() == 65);

	print_dbg("");
	while(bs.available() != 0) {
		printf("%c", bs.read());
	}
	printf("\n");

	lwiot_destroy();

	wait_close();
	return -EXIT_SUCCESS;
}
