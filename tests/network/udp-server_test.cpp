/*
 * UDP server unit test.
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

#include <lwiot/network/udpclient.h>
#include <lwiot/network/udpserver.h>
#include <lwiot/network/socketudpserver.h>

#include <lwiot/stl/move.h>

static void start_and_run_server()
{
	char buffer[256];
	bool test;
	int length;
	lwiot::SocketUdpServer udp(BIND_ADDR_LB, 5000);
	lwiot::UniquePointer<lwiot::UdpClient> client;

	test = udp.bind();
	assert(test);
	memset(buffer, 0, sizeof(buffer));
	auto size = sizeof(buffer);
	client = lwiot::stl::move( udp.recv(buffer, size) );

	length = strlen(buffer);
	assert(length > 0);
	print_dbg("Received from client: %s\n", buffer);
	client->write(buffer, strlen(buffer));
	client->close();
}

int main(int argc, char **argv)
{
	lwiot_init();

	print_dbg("Testing UdpServer implementation!\n");
	start_and_run_server();
	lwiot_destroy();
	wait_close();

	return -EXIT_SUCCESS;
}
