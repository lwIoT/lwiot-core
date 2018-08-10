/*
 * lwIoT initialisation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdio.h>

#include <lwiot/lwiot.h>
#include <lwiot/log.h>

void lwiot_init(void)
{
	log_init(stdout);
	lwiot_timers_init();

#ifdef NO_OS
	no_os_init();
#endif
	print_dbg("lwIoT initialised!\n");
}

void lwiot_destroy(void)
{
	lwiot_timers_destroy();
}
