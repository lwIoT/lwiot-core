/*
 * Generic TCP server unit test.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>
#include <assert.h>

#include <lwiot/log.h>
#include <lwiot/tcpserver.h>
#include <lwiot/dns.h>
#include <lwiot/test.h>

static void test_tcpserver(void)
{
	lwiot::TcpServer server;
	lwiot::IPAddress addr(0,0,0,0);
	unsigned char buffer[1024];

	assert(server.bind(addr, 5555));
	auto client = server.accept();
	assert(client.connected());

	auto num = client.read(buffer, sizeof buffer);
	assert(num > 0);
	client.write(buffer, num);

	client.close();
	server.close();
}

int main(int argc, char **argv)
{
	lwiot_init();

	print_dbg("Testing TcpServer implementation!\n");
	test_tcpserver();
	lwiot_destroy();
	wait_close();

	return -EXIT_SUCCESS;
}
