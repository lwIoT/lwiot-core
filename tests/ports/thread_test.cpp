/*
 * Threading unit test.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/kernel/atomic.h>

#ifdef HAVE_RTOS
#include <FreeRTOS.h>
#include <task.h>
#endif

#include <lwiot/log.h>
#include <lwiot/kernel/thread.h>
#include <lwiot/kernel/functionalthread.h>
#include <lwiot/test.h>
#include <lwiot/kernel/lock.h>

lwiot::atomic_long_t counter;

class ThreadTest : public lwiot::Thread {
public:
	explicit ThreadTest(lwiot::Lock *lock) : Thread("Testing thread", (void*)lock)
	{
	}

protected:
	void run() override
	{
		auto *lock = (lwiot::Lock*) this->_argument;

		const char *arg = "test-tp";
		int i = 0;

		while(i++ <= 5) {
			lock->lock();
			print_dbg("[%s] Thread is running!\n", arg);
			lock->unlock();

			counter++;
			lwiot_sleep(1000);
		}
	}
};

static void main_thread(void *arg)
{
	static auto *lock = (lwiot::Lock*)arg;

	lwiot::Function<void(void)> lambda1 = [&]() -> void {
		int i = 0;

		while(i++ <= 5) {
			lock->lock();
			print_dbg("Lambda thread 1 ping!\n");
			lock->unlock();

			counter--;
			lwiot_sleep(750);
		}
	};

	lwiot::Function<void(void)> lambda2 = [&]() -> void {
		int i = 0;

		while(i++ <= 5) {
			auto value = counter.load();
			lock->lock();
			print_dbg("Lambda thread 2 ping! Atomic: %li\n", value);
			lock->unlock();
			lwiot_sleep(750);
		}
	};

	lock->lock();
	lwiot::FunctionalThread tp1("ft-tp1", lambda1);
	lwiot::FunctionalThread tp2("ft-tp2", lambda2);
	tp1.start();
	tp2.start();
	lock->unlock();

	lwiot_sleep(6000);
	tp1.stop();
	tp2.stop();
#ifdef HAVE_RTOS
	vTaskEndScheduler();
#endif
}

int main(int argc, char **argv)
{
	lwiot_thread_t *tp;

	lwiot_init();

	lwiot::Lock lock(false);
	print_dbg("Creating threads..\n");

	ThreadTest t1(&lock), t2(&lock);
	
	tp = lwiot_thread_create(main_thread, "main" , &lock);

	t1.start();
	t2.start();

#ifdef HAVE_RTOS
	vTaskStartScheduler();
#else
	t1.join();
	t2.join();
#endif

	//lwiot_sleep(8000);
	lwiot_thread_destroy(tp);

	lwiot_destroy();
	wait_close();
}
