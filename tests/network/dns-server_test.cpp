/*
 * DNS server unit test.
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
#include <lwiot/network/dnsserver.h>

#include <lwiot/stl/move.h>

static void start_and_run_server()
{
	auto udp = new lwiot::SocketUdpServer(BIND_ADDR_LB, 5000);
	lwiot::DnsServer srv;

	srv.map("test.com", lwiot::IPAddress(127,0,0,1));
	srv.map("www.test.com", lwiot::IPAddress(127,0,0,1));
	udp->bind();
	srv.begin(udp);

	lwiot_sleep(1000);
	wait_close();
	srv.end();
	srv.join();
	print_dbg("DNS server test completed!\n");
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
