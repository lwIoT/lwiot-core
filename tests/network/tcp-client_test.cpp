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
#include <lwiot/network/tcpclient.h>
#include <lwiot/network/sockettcpclient.h>
#include <lwiot/test.h>

static void test_tcpclient()
{
	lwiot::SocketTcpClient client;
	uint8_t data[] = {'a', 'b', 'c', 'd'};
	uint8_t buffer[4];

	lwiot::IPAddress addr(127, 0, 0, 1);

	assert(client.connect(addr, 5555));
	client.write(data, sizeof(data));
	client.read(buffer, sizeof(buffer));

	assert(memcmp(data, buffer, sizeof(buffer)) == 0);
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
