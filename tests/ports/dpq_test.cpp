/*
 * Dispatch queue unit test.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <lwiot.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include <lwiot/util/application.h>
#include <lwiot/functor.h>
#include <lwiot/log.h>
#include <lwiot/test.h>

#include <lwiot/kernel/dispatchqueue.h>
#include <lwiot/kernel/atomic.h>

#include <lwiot/detail/dpq_policy.h>
#include <lwiot/detail/dpq_base.h>

#ifdef HAVE_RTOS
#include <FreeRTOS.h>
#include <task.h>
#endif

typedef lwiot::DispatchQueue<bool(int), lwiot::MultiThreadingPolicy> MultiThreadingQueue;

class Application : public lwiot::Functor {
	void run() override
	{
		print_dbg("Inside init test..\n");
		lwiot::AtomicBool has_run;
		lwiot::atomic_int_t event_1(0);
		lwiot::atomic_int_t event_2(0);
		lwiot::atomic_int_t event_3(0);

		lwiot::DispatchQueue<bool(int), lwiot::MultiThreadingPolicy> dpq_multi("io1");
		lwiot::DispatchQueue<bool(int), lwiot::MultiThreadingPolicy> swapper("io2");
		lwiot::DispatchQueue<void(void), lwiot::SingleThreadingPolicy> dpq_single;

		const MultiThreadingQueue::HandlerType handler = [&](int x) {
			event_1 += 1;
			return true;
		};

		swapper.enqueue(handler, 1);
		swapper.enqueue([&](int x) {
			event_2 += 1;
			return true;
		}, 2);

		swapper.enqueue([&](int x) {
			bool ran_before = has_run;
			has_run.store(true);
			event_3 += 1;

			return ran_before;
		}, 3);

		dpq_multi = lwiot::stl::move(swapper);
		dpq_multi.enable();

		lwiot_sleep(500);
		fflush(stdout);

		swapper = lwiot::stl::move(dpq_multi);

		swapper.enable();

		dpq_multi.enqueue(handler, 1);
		swapper.enqueue(handler, 1);

		lwiot_sleep(500);
		fflush(stdout);

		assert(dpq_multi.size() == 0UL);
		assert(swapper.size() == 0UL);

		assert(event_1.load() == 3);
		assert(event_2.load() == 1);
		assert(event_3.load() == 2);

		dpq_single.enable();
		dpq_single.disable();
		dpq_multi.disable();
	}
} app;

static void start_app()
{
	lwiot::Application testapp(app);
	testapp.start();
}

static void main_thread(void *arg)
{
	start_app();
	wait_close();
#ifdef HAVE_RTOS
	vTaskEndScheduler();
#endif
}

int main(int argc, char **argv)
{
	lwiot_thread_t *tp;
	printf("Running lwIoT init test!\n");

	tp = lwiot_thread_create(main_thread, "main" , nullptr);

#ifdef HAVE_RTOS
	vTaskStartScheduler();
#endif
	lwiot_thread_destroy(tp);

	return -EXIT_SUCCESS;
}
