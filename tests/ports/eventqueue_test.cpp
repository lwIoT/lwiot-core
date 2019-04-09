/*
 * Event queue unit test.
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
#include <lwiot/types.h>
#include <lwiot/test.h>

#include <lwiot/kernel/thread.h>
#include <lwiot/kernel/eventqueue.h>
#include <lwiot/kernel/lock.h>

class TestBus {
public:
	TestBus() : _lock(new lwiot::Lock(false))
	{
	}

	TestBus(const TestBus&) = delete;
	TestBus& operator=(const TestBus&) = delete;

	virtual ~TestBus() = default;

	void test() const
	{
		lwiot::ScopedLock g(*this->_lock);
		print_dbg("Testing bus..\n");
	}

private:
	lwiot::SharedPointer<lwiot::Lock> _lock;
};

static bool hello_world_event(time_t time)
{
	print_dbg("Hello, World event triggered!\n");
	return true;
}

static int x = 0;

class TestApplication {
public:
	TestApplication() : y(10)
	{
	}

	void evq_test()
	{
		lwiot::EventQueue<bool(time_t)> evq;

		evq.on("hello", &hello_world_event);
		evq.on("tmp", [&](time_t time) {
			this->_bus.test();
			return true;
		});

		time_t time = lwiot_tick_ms();
		lwiot::Function<bool(time_t)> func = [&](time_t t) {
			(void)t;
			print_dbg("Value of y: %i\n", this->y);
			this->_bus.test();
			return true;
		};

		evq.enqueue(func, lwiot::stl::move(time));

		evq.on("test", [&](time_t t) -> bool {
			(void)t;
			print_dbg("Test event!\n");
			x++;

			return x == 4;
		});

		evq.signal("test");
		evq.signal("hello");
		evq.enable();

		evq.signal("hello");
		evq.signal("tmp");

		print_dbg("Value of X: %i\n", x);
		lwiot_sleep(1000);
		evq.remove("tmp");
	}

private:
	TestBus _bus;
	int y;
};

lwiot::atomic_long_t counter;

static void main_thread(void *arg)
{
	TestApplication app;
	app.evq_test();

	++counter;
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
