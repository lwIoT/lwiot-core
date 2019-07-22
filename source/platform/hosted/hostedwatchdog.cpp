/*
 * Hosted watchdog implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/io/watchdog.h>
#include <lwiot/hosted/hostedwatchdog.h>

void lwiot::hosted::Watchdog::reset()
{
}

lwiot::Watchdog& wdt = lwiot::hosted::Watchdog::instance();
