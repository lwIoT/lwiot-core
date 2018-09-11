/*
 * Threading unit test.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/log.h>
#include <lwiot/thread.h>
#include <lwiot/functionalthread.h>
#include <lwiot/test.h>
#include <lwiot/lock.h>

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

static void main_thread(void *arg)
{
	lwiot::Lock lock(false);

	lwiot::Function<void(*)(void)> lambda1 = [&]() -> void {
		int i = 0;

		lock.lock();
		while(i++ <= 5) {
			print_dbg("Lambda thread 1 ping!\n");
			lwiot_sleep(750);
		}
		lock.unlock();
	};

	lwiot::Function<void(*)(void)> lambda2 = [&]() -> void {
		int i = 0;

		lock.lock();
		while(i++ <= 5) {
			print_dbg("Lambda thread 2 ping!\n");
			lwiot_sleep(750);
		}
		lock.unlock();
	};

	lwiot::FunctionalThread tp1("ft-tp1", lambda1);
	lwiot::FunctionalThread tp2("ft-tp2", lambda2);
	tp1.start();
	tp2.start();

	lwiot_sleep(6000);
	tp1.stop();
	tp2.stop();
#ifdef HAVE_RTOS
	vTaskEndScheduler();
#endif
}

int main(int argc, char **argv)
{
	lwiot_thread_t tp;

	lwiot_init();
	print_dbg("Creating threads..\n");

	ThreadTest t1("Thread 1"),
		t2("Thread 2");
	
	strcpy(tp.name, "main");
	tp.name[sizeof("main")] = '\0';
	lwiot_thread_create(&tp, main_thread, NULL);

	t1.start();
	t2.start();

#ifdef HAVE_RTOS
	vTaskStartScheduler();
#endif

	t1.stop();
	t2.stop();
	lwiot_thread_destroy(&tp);

	lwiot_destroy();
	wait_close();
}
