/*
 * Captive portal unit test.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/error.h>
#include <lwiot/scopedlock.h>
#include <lwiot/test.h>

#include <lwiot/network/udpclient.h>
#include <lwiot/network/udpserver.h>
#include <lwiot/network/socketudpserver.h>
#include <lwiot/network/captiveportal.h>

#include <lwiot/stl/move.h>

static void start_and_run_server()
{
	lwiot::SocketUdpServer* srv = new lwiot::SocketUdpServer();
	lwiot::CaptivePortal portal(lwiot::IPAddress(lwiot::IPAddress::fromString("1.2.3.4")));

	portal.begin(srv, 5300);
	lwiot_sleep(5000);
	portal.end();
	print_dbg("Captive portal test complete!");
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
