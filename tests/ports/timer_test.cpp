/*
 * Unit test for the Timer class.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <assert.h>
#include <lwiot.h>

#ifdef HAVE_RTOS
#include <FreeRTOS.h>
#include <task.h>
#endif

#include <lwiot/kernel/timer.h>
#include <lwiot/kernel/functionaltimer.h>
#include <lwiot/kernel/thread.h>
#include <lwiot/log.h>
#include <lwiot/test.h>

#ifdef NDEBUG
#error "Debugging not enabled.."
#endif

class TestTimer : public lwiot::Timer {
public:
	TestTimer(const lwiot::String& name, unsigned long ms, uint32_t flags, void *arg)
		: Timer(name, ms, flags, arg), _ticks(0)
	{ }

	const int& ticks() const
	{
		return this->_ticks;
	}

protected:
	void tick() override
	{
		print_dbg("Timer tick..\n");
		_ticks++;
	}

private:
	int _ticks;
};

class ThreadTest : public lwiot::Thread {
public:
	explicit ThreadTest(const char *arg) : Thread("Testing thread", (void*)arg)
	{
	}

protected:
	void run() override
	{
		TestTimer *timer;

		timer = new TestTimer("Test tmr", 500, 0, nullptr);
		lwiot_sleep(1000);
		assert(timer->ticks() == 0);

		timer->start();
		lwiot_sleep(2010);
		assert(timer->ticks() == 4);

		for(int idx = 0; idx < 4; idx++) {
			timer->reset();
			lwiot_sleep(250);
		}

		assert(timer->ticks() == 4);
		lwiot_sleep(510);
		assert(timer->ticks() == 5);

		timer->stop();
		lwiot_sleep(1000);
		delete timer;

		lwiot::FunctionalTimer<void>  ft1(lwiot::TimerType::OneShot, 2500);
		lwiot::FunctionalTimer<int>   ft2(lwiot::TimerType::OneShot, 2000, 2);

		ft1.start([&]() {
			print_dbg("Timer 1 triggered!\n");
		});

		ft2.start([&](int x) {
			print_dbg("Timer %i triggered!\n", x);
		});

		lwiot_sleep(3000);

#ifdef HAVE_RTOS
		vTaskEndScheduler();
#endif
	}
};

int main(int argc, char **argv)
{
	lwiot_init();
	UNUSED(argc);
	UNUSED(argv);

	ThreadTest t1("timer-test");

	t1.start();
#ifdef HAVE_RTOS
	vTaskStartScheduler();
#endif

	wait_close();
	lwiot_destroy();

	return -EXIT_SUCCESS;
}
