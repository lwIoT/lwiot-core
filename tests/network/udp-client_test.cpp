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
#include <lwiot/network/socketudpclient.h>
#include <lwiot/network/ntpclient.h>

#include <lwiot/util/datetime.h>
#include <lwiot/stl/move.h>

static void test_udp_client()
{
	lwiot::SocketUdpClient io("pool.ntp.org", 123);
	lwiot::NtpClient client;

	client.begin(io);
	print_dbg("Testing NTP client\n");

	client.update();
	lwiot::DateTime dt(client.time());
	print_dbg("Date time: %s\n", dt.toString().c_str());
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
