/*
 * Threading unit test.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/log.h>
#include <lwiot/thread.h>
#include <lwiot/test.h>

class ThreadTest : public lwiot::Thread {
public:
	explicit ThreadTest(const char *arg) : Thread("Testing thread", (void*)arg)
	{
	}

protected:
	void run(void *_argument) override
	{
		const char *arg = (const char *)_argument;
		int i = 0;

		while(i++ <= 5) {
			print_dbg("[%s] Thread is running!\n", arg);

			lwiot_sleep(1000);
		}
	}
};

int main(int argc, char **argv)
{
	lwiot_init();
	print_dbg("Creating threads..\n");

	ThreadTest t1("Thread 1"),
		t2("Thread 2");

	t1.start();
	t2.start();

#ifdef HAVE_RTOS
	vTaskStartScheduler();
#endif

	t1.stop();
	t2.stop();

	lwiot_destroy();
	wait_close();
}
