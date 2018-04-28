/*
 * lwIoT initialisation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <lwiot/log.h>

void lwiot_init(void)
{
	log_init(stdout);
}
