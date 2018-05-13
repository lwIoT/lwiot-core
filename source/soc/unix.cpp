/*
 * SoC specific functions
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <unistd.h>
#include <lwiot.h>

#include <lwiot/types.h>

void udelay(uint32_t us)
{
	usleep(us);
}
