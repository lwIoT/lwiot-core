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
	print_dbg("lwIoT initialised!");
}

void lwiot_destroy(void)
{
	lwiot_timers_destroy();
}
