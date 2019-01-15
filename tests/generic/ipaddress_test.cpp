/*
 * IPAddress unit test.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#ifdef WIN32
#include <Winsock2.h>
#else
#include <arpa/inet.h>
#endif

#include <lwiot/log.h>
#include <lwiot/test.h>
#include <lwiot/network/ipaddress.h>

int main(int argc, char **argv)
{
	lwiot_init();

	lwiot::IPAddress addr1((uint32_t)htonl(0xFFAA0010));
	lwiot::IPAddress addr2 = lwiot::IPAddress::fromString("127.0.0.1");
	lwiot::IPAddress addr3(255,255,0,1);

	print_dbg("IP addresses:\n");
	print_dbg("Address 1: %s\n", addr1.toString().c_str());
	print_dbg("Address 2: %s\n", addr2.toString().c_str());
	print_dbg("Address 3: %s\n", addr3.toString().c_str());

	wait_close();
	lwiot_destroy();
	return -EXIT_SUCCESS;
}
