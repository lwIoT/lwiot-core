/*
 * lwIoT initialisation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdio.h>
#include <lwiot.h>

#include <lwiot/lwiot.h>
#include <lwiot/log.h>

#ifdef WIN32
#include <WinSock2.h>
#endif

void lwiot_init(void)
{
	srand(time(NULL));
	log_init(stdout);
	lwiot_timers_init();

#ifdef CONFIG_STANDALONE
	no_os_init();
#else
	/* Create the DNS event queue */
#ifdef HAVE_LWIP
	lwiot_dns_event = lwiot_event_create(4);
#endif
#endif

#ifdef WIN32
	WSADATA data;
	int result;

	result = WSAStartup(MAKEWORD(2, 2), &data);
	if(result) {
		print_dbg("Failed to initialize WinSock2.\n");
		return;
	}

#endif
	print_dbg("lwIoT initialised!\n");
}

void lwiot_destroy(void)
{
	lwiot_timers_destroy();
#ifdef WIN32
	WSACleanup();
#endif
}
