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
#include <lwiot/network/dns.h>
#include <lwiot/test.h>

#include <lwiot/network/tcpclient.h>
#include <lwiot/network/tcpserver.h>
#include <lwiot/network/sockettcpclient.h>
#include <lwiot/network/sockettcpserver.h>

#include <lwiot/stl/move.h>

static void test_tcpserver()
{
	lwiot::SocketTcpServer server;
	unsigned char buffer[1024];

	assert(server.bind(BIND_ADDR_ANY, 5555));
	lwiot::UniquePointer<lwiot::TcpClient> client = lwiot::stl::move(server.accept());
	assert(client->connected());

	auto num = client->read(buffer, sizeof buffer);
	assert(num > 0);
	client->write(buffer, num);

	client->close();
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
