/*
 * Events wrapper unit test.
 *
 * @author Michel Megens
 * @email dev@bietje.net
 */

#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <lwiot.h>

#ifdef HAVE_RTOS
#include <FreeRTOS.h>
#include <task.h>
#endif

#include <lwiot/event.h>
#include <lwiot/log.h>
#include <lwiot/thread.h>
#include <lwiot/test.h>

class ThreadTest : public lwiot::Thread {
public:
	explicit ThreadTest(const char *arg, lwiot::Event& event) : Thread("Testing thread", (void*)arg)
	{
		this->event = &event;
	}

protected:
	void run(void *_argument) override
	{
		const char *arg = (const char *)_argument;

		print_dbg("[%s] Thread is running!\n", arg);
		this->event->wait();
		print_dbg("[%s] Thread is signalled!\n", arg);
	}

private:
	lwiot::Event *event;
};

static void main_thread(void *arg)
{
	lwiot::Event *e = (lwiot::Event*)arg;

	lwiot_sleep(1000);
	e->signal();
	lwiot_sleep(1000);
	e->signal();
	lwiot_sleep(1000);

#ifdef HAVE_RTOS
	vTaskEndScheduler();
#endif
}

int main(int argc, char **argv)
{
	lwiot_thread_t *tp;

	lwiot_init();
	print_dbg("Creating threads..\n");
	lwiot::Event e;

	ThreadTest t1("Thread 1", e),
		t2("Thread 2", e);

	t1.start();
	t2.start();
	tp = lwiot_thread_create(main_thread, "main" , nullptr);

#ifdef HAVE_RTOS
	vTaskStartScheduler();
#endif

	t1.stop();
	t2.stop();
	lwiot_thread_destroy(tp);
	lwiot_destroy();

	wait_close();
	return -EXIT_SUCCESS;
}
