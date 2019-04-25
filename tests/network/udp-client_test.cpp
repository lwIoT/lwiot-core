/*
 * UDP client unit test.
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
#include <lwiot/network/socketudpclient.h>

#include <lwiot/stl/move.h>

static void test_udp_client()
{
	char buffer[] = "Hello, World!";
	char readback[sizeof(buffer) + 1];
	//auto ip = lwiot::stl::move(lwiot::IPAddress::fromString("127.0.0.1"));
	lwiot::IPAddress ip(127, 0, 0, 1);
	lwiot::SocketUdpClient client(ip, 5000);

	assert(client.write(buffer, sizeof(buffer)) > 0);
	memset(readback, 0, sizeof(readback));

	while(client.available() == 0);
	assert(client.read(readback, sizeof(readback)) > 0);
	print_dbg("Read back: %s\n", readback);
	//client.close();
}

int main(int argc, char **argv)
{
	lwiot_init();

	print_dbg("Testing TcpServer implementation!\n");
	test_udp_client();
	lwiot_destroy();
	wait_close();

	return -EXIT_SUCCESS;
}
