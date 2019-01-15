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

void lwiot_init(void)
{
	srand(time(NULL));
	log_init(stdout);
	lwiot_timers_init();

#ifdef NO_OS
	no_os_init();
#else
	/* Create the DNS event queue */
#ifdef HAVE_LWIP
	lwiot_dns_event = lwiot_event_create(4);
#endif
#endif
	print_dbg("lwIoT initialised!\n");
}

void lwiot_destroy(void)
{
	lwiot_timers_destroy();
}
