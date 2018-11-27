/*
 * SoC specific functions
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <unistd.h>
#include <lwiot.h>
#include <pthread.h>

#include <lwiot/types.h>

static pthread_mutex_t crit_section_mtx = PTHREAD_MUTEX_INITIALIZER;

void lwiot_udelay(uint32_t us)
{
	usleep(us);
}

void enter_critical()
{
	pthread_mutex_lock(&crit_section_mtx);
}

void exit_critical()
{
	pthread_mutex_unlock(&crit_section_mtx);
}
