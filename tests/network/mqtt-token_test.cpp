/*
 * Unit test for MQTT tokens.
 *
 * @auther Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <assert.h>
#include <lwiot.h>

#ifdef HAVE_RTOS
#include <FreeRTOS.h>
#include <task.h>
#endif

#include <lwiot/log.h>
#include <lwiot/types.h>
#include <lwiot/linkedlist.h>
#include <lwiot/functor.h>
#include <lwiot/test.h>
#include <lwiot/application.h>
#include <lwiot/thread.h>

#include "../../source/net/mqtt/mqtttoken.h"
#include "../../source/net/mqtt/mqtttokenbucket.h"

class ThreadTest : public lwiot::Thread {
public:
	explicit ThreadTest() : Thread("Testing thread", nullptr), _event(3), _bucket()
	{
	}

	void execute()
	{
		lwiot::Event e;
		lwiot::ScopedLock lock(this->_lock);

		lwiot::MqttToken token = lwiot::stl::move(this->_bucket.create(e, 12));
		this->_event.signal();

		auto status = e.wait(lock, 1000);
		assert(status);
		token.clear();
	}

protected:
	void run() override
	{
		for(int idx = 0; idx < 2; idx++) {
			lwiot::ScopedLock lock(this->_lock);

			this->_event.wait(lock, FOREVER);
			lwiot::MqttToken token = lwiot::stl::move(this->_bucket.front());
			token.event.signal();
			print_dbg("PING\n");
		}
	}

private:
	lwiot::Event _event;
	lwiot::MqttTokenBucket _bucket;
	lwiot::Lock _lock;
};

class MqttTokenApplication : public lwiot::Functor {
protected:
	void run() override
	{
		ThreadTest tp;
		tp.start();

		for(int idx = 0; idx < 2; idx++) {
			tp.execute();
			lwiot_sleep(1000);
		}

		tp.stop();
	}
};

static void main_thread(void *arg)
{
	lwiot::Application *app = (lwiot::Application*)arg;
	app->start();

#ifdef HAVE_RTOS
	vTaskEndScheduler();
#endif
}

int main(int argc, char** argv)
{
	lwiot_thread_t* tp;

	{
		MqttTokenApplication runner;
		lwiot::Application app(runner);
		tp = lwiot_thread_create(main_thread, "main", &app);


#ifdef HAVE_RTOS
		vTaskStartScheduler();
#else
		lwiot_sleep(5000);
#endif

		lwiot_thread_destroy(tp);
	}

	wait_close();

	return -EXIT_SUCCESS;
}
