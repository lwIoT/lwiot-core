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
	print_dbg("lwIoT initialised!\n");
}
