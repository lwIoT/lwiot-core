/*
 * Event queue unit test.
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

#include <lwiot/kernel/eventqueue.h>
#include <lwiot/kernel/atomic.h>
#include <lwiot/kernel/timereventqueue.h>

#include <lwiot/util/datetime.h>
#include <lwiot/stl/string.h>

#ifdef HAVE_RTOS
#include <FreeRTOS.h>
#include <task.h>
#endif

class Application : public lwiot::Functor {
	static void test_timer_queue()
	{
		lwiot::TimerEventQueue evq;
		lwiot::atomic_int_t counter(0);
		lwiot::AtomicBool triggered(false);
		lwiot::DateTime dt;

		dt += 3;

		evq.every(1000, "test", [&counter, &triggered](time_t system, time_t local) {
			++counter;
			bool is_ok = triggered;
			triggered = true;
			return is_ok;
		});

		evq.on("single", [&](time_t system, time_t local) {
			counter++;
			return true;
		});

		evq.at(dt, "single");

		time_t start = lwiot_tick_ms();
		time_t now = 0;

		for(time_t idx = start; idx < (start+5900); idx = lwiot_tick_ms()) {
			if(now != idx) {
				evq.feed();
				evq.process_ticks();
				now = idx;
			}
		}

		print_dbg("Processing took: %lu\n", now - start);
		print_dbg("Counter: %i\n", counter.load());

		assert(counter.load() == 7);
	}

	void run() override
	{
		test_timer_queue();

		lwiot::EventQueue<lwiot::String, bool(lwiot::DateTime)> original;
		lwiot::EventQueue<lwiot::String, bool(lwiot::DateTime)> evq;
		lwiot::AtomicBool triggered(false);

		evq.enable();
		original.enable();
		original.on("test", [&](lwiot::DateTime dt) {
			triggered = true;
			return true;
		});

		evq = lwiot::stl::move(original);

		evq.signal("test");
		lwiot_sleep(500);
		evq.remove("test");

		assert(triggered == true);
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
