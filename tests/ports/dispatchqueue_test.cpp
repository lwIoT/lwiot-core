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
#include <lwiot/test.h>

#include <lwiot/kernel/thread.h>
#include <lwiot/kernel/functionalthread.h>
#include <lwiot/kernel/dispatchqueue.h>
#include <lwiot/kernel/eventqueue.h>
#include <lwiot/kernel/lock.h>

lwiot::atomic_long_t counter;

static void test_dpq()
{
	print_dbg("Main thread started..!\n");
	lwiot::DispatchQueue<bool(void), lwiot::MultiThreadingPolicy> dpq;

	dpq.enqueue([]() -> bool {
		print_dbg("Work item 1 done!\n");
		return true;
	});

	int x = 0;
	dpq.enqueue([&x]() -> bool {
		print_dbg("Work item 2 done!\n");
		x++;

		return x == 5;
	});

	dpq.enable();

	dpq.enqueue([]() -> bool {
		print_dbg("Work item 3 done!\n");
		return true;
	});

	dpq.enqueue([]() -> bool {
		print_dbg("Work item 4 done!\n");
		return true;
	});

	lwiot_sleep(100);

	dpq.enqueue([]() -> bool {
		print_dbg("Work item 5 done!\n");
		return true;
	});

	lwiot_sleep(1000);
	++counter;
}

static void main_thread(void *arg)
{
	test_dpq();

#ifdef HAVE_RTOS
	vTaskEndScheduler();
#endif
}

int main(int argc, char **argv)
{
	lwiot_thread_t *tp;

	lwiot_init();
	print_dbg("Creating threads..\n");

	tp = lwiot_thread_create(main_thread, "main" , nullptr);

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
