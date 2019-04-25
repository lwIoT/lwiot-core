/*
 * Dispatch queue test.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <lwiot.h>

#ifdef HAVE_RTOS
#include <FreeRTOS.h>
#include <task.h>
#endif

#include <lwiot/log.h>
#include <lwiot/kernel/thread.h>
#include <lwiot/kernel/functionalthread.h>
#include <lwiot/kernel/dispatchqueue.h>
#include <lwiot/test.h>
#include <lwiot/kernel/lock.h>

lwiot::atomic_long_t counter;

static void main_thread(void *arg)
{
	print_dbg("Main thread started..!\n");
	counter++;

#ifdef HAVE_RTOS
	vTaskEndScheduler();
#endif
}

using Threading = typename lwiot::detail::SelectThreadingPolicy<
		lwiot::detail::MultiThreadingPolicy<int>, lwiot::detail::HasThreading<lwiot::detail::MultiThreadingPolicy<int>>::value
>::Type ;

int main(int argc, char **argv)
{
	lwiot_thread_t *tp;

	lwiot_init();
	print_dbg("Creating threads..\n");

	tp = lwiot_thread_create(main_thread, "main" , nullptr);

	static_assert(!lwiot::detail::HasThreading<lwiot::detail::SingleThreadPolicy<int>>::value, "Unexpected support for threading!");
	static_assert(lwiot::detail::HasThreading<lwiot::detail::MultiThreadingPolicy<int>>::value, "Threading not supported!");

#ifdef HAVE_RTOS
	vTaskStartScheduler();
#else
	while(counter.load() == 0)
		lwiot_sleep(1);
#endif

	lwiot_thread_destroy(tp);
	lwiot_destroy();
	wait_close();
}
