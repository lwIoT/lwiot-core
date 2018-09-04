/*
 * Generic TCP client unit test.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>
#include <assert.h>

#include <lwiot/log.h>
#include <lwiot/tcpclient.h>
#include <lwiot/test.h>

static void test_tcpclient(void)
{
	lwiot::TcpClient client;
	uint8_t data[] = {'a', 'b', 'c', 'd'};
	uint8_t stop[] = {'q', 'u', 'i', 't', '\r', '\n'};
	uint8_t buffer[4];

	assert(client.connectTo(lwiot::IPAddress(127, 0, 0, 1), 5555));
	client.write(data, sizeof(data));
	client.read(buffer, sizeof(buffer));

	assert(memcmp(data, buffer, sizeof(buffer)) == 0);
	client.write(stop, sizeof(stop));
	client.close();
}

int main(int argc, char **argv)
{
	lwiot_init();
	test_tcpclient();
	lwiot_destroy();
	wait_close();

	return -EXIT_SUCCESS;
}
