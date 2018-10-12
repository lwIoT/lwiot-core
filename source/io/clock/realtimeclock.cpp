/*
 * RTC interface / abstract class implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/log.h>
#include <lwiot/realtimeclock.h>

void lwiot::RealTimeClock::time(lwiot::DateTime &dt)
{
	dt = this->now();
}
