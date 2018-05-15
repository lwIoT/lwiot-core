/*
 * ByteBuffer unit test.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <assert.h>
#include <lwiot.h>

#include <lwiot/bytebuffer.h>
#include <lwiot/test.h>
#include <lwiot/log.h>

int main(int argc, char **argv)
{
	lwiot::ByteBuffer bf;

	lwiot_init();
	for(int i = 65; i < 75; i++)
		bf.write((uint8_t)i);

	print_dbg("Bytes in buffer: ");
	for(auto x : bf) {
		printf("%c ", x);
	}

	putc('\n', stdout);
	print_dbg("Bytes in second buffer: ");
	lwiot::ByteBuffer bf2(bf);
	for(auto x : bf2) {
		printf("%c ", x);
	}

	assert(bf == bf2);

	putc('\n', stdout);
	lwiot_destroy();

	wait_close();
	return -EXIT_SUCCESS;
}

