/*
 * SoC specific functions
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/types.h>

#include <thread>
#include <chrono>

extern "C" void lwiot_udelay(uint32_t us)
{
	std::this_thread::sleep_for(std::chrono::microseconds(us));
}
