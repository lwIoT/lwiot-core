//
// Created by michel on 28/04/18.
//

#include <lwiot/thread.h>
#include <lwiot/log.h>

class ThreadTest : public lwiot::Thread {
public:
	explicit ThreadTest() : Thread("Testing thread")
	{
	}

protected:
	void run(void *argument) override
	{
		while(true) {
			print_dbg("Thread is running!\n");

			lwiot_sleep(1000);
		}
	}
};

int main(int argc, char **argv)
{
	lwiot_init();
	print_dbg("Creating threads..\n");

	ThreadTest t1, t2;

	t1.start();
	t2.start();

	vTaskStartScheduler();

	while(true);
}
