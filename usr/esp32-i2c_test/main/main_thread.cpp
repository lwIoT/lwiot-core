/*
 * ESP32 device test.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <esp_attr.h>
#include <esp_heap_caps.h>
#include <lwiot.h>

#include <lwiot/thread.h>
#include <lwiot/log.h>
#include <lwiot/string.h>
#include <lwiot/gpiochip.h>
#include <lwiot/gpiopin.h>
#include <lwiot/watchdog.h>
#include <lwiot/datetime.h>
#include <lwiot/i2cbus.h>
#include <lwiot/softwarei2calgorithm.h>
#include <lwiot/application.h>
#include <lwiot/functionalthread.h>

#include <lwiot/esp32/esp32pwm.h>
#include <lwiot/esp32/esp32i2calgorithm.h>

static int calculate_udelay_test(const uint32_t& frequency)
{
	double freq, tval;

	if(frequency == 0U)
		return -EINVALID;

	freq = static_cast<double>(frequency);
	tval = 1.0 / (2.0 * freq);
	tval -= 0.000002;
	tval *= 1e6;

	return static_cast<int>(tval);
}

class I2CTestApplication : public lwiot::Functor {
public:
	virtual void operator()() override
	{
		size_t freesize;
		auto algo = new lwiot::SoftwareI2CAlgorithm(23, 22, 100000U);
		lwiot::I2CBus bus(algo);

		printf("Main thread started!\n");
		lwiot_sleep(1000);

		lwiot::DateTime dt;
		print_dbg("Time: %s\n", dt.toString().c_str());
		freesize = heap_caps_get_free_size(0);
		wdt.enable();

		print_dbg("Free heap size: %u\n", freesize);
		print_dbg("Software I2C udelay value: %i\n", calculate_udelay_test(100000UL));

		print_dbg("Testing I2C bus..\n");
		algo->test();

		FUNC_THREAD(td, "functp", []() -> void {
			int idx = 0;

			while(idx++ < 5) {
				print_dbg("[ftp] Thread ping..\n");
				lwiot_sleep(750);
			}
		});

		td.start();

		while(true) {
			lwiot::Vector<lwiot::I2CMessage*> msgs;
			lwiot::I2CMessage wr(1);
			lwiot::I2CMessage rd(32);
			lwiot::I2CMessage rd2(32);

			wr.setRepeatedStart(true);
			wr.setAddress(0x5B, false, false);
			wr.write(2);
			msgs.pushback(&wr);

			rd.setAddress(0x5B, false, false);
			rd.setRepeatedStart(true);
			rd2.setAddress(0x5B, false, false);
			rd2.setRepeatedStart(false);

			for(int i = 0; i < 16; i++) {
				rd.write('a');
				rd.write('b');

				rd2.write('c');
				rd2.write('d');
			}

			msgs.pushback(&rd);
			msgs.pushback(&rd);
			msgs.pushback(&rd);
			msgs.pushback(&rd);
			msgs.pushback(&rd);
			msgs.pushback(&rd);

			msgs.pushback(&rd2);

			print_dbg("Writing I2C msg\n");
			if(!bus.transfer(msgs)) {
				print_dbg("Failed to write I2C bus!\n");
			}

			lwiot_sleep(120);
			this->testRead(bus);
			lwiot_sleep(50);
			this->testSingle(bus);

			wdt.reset();
			lwiot_sleep(500);
		}
	}

private:
	void testSingle(lwiot::I2CBus& bus)
	{
		lwiot::I2CMessage wr(1);

		wr.setAddress(0x5B, false, false);
		wr.write(1);

		if(bus.transfer(wr)) {
			print_dbg("Single test successfull!\n");
		} else {
			print_dbg("Single test failed!\n");
		}

		lwiot_sleep(500);
	}

	void testRead(lwiot::I2CBus& bus)
	{
		lwiot::I2CMessage wr(1), rd(3);
		lwiot::Vector<lwiot::I2CMessage*> msgs;

		wr.setAddress(0x5B, false, false);
		wr.write(1);
		wr.setRepeatedStart(true);

		rd.setAddress(0x5B, false, true);

		msgs.pushback(&wr);
		msgs.pushback(&rd);

		if(bus.transfer(msgs)) {
			print_dbg("Read test successfull!\n");
		} else {
			print_dbg("Read test failed!\n");
		}

		auto& msg = *msgs[1];
		print_dbg("Read data:\n");
		for(auto& byte : msg) {
			print_dbg("Read byte: %u\n", byte);
		}

		lwiot_sleep(500);
	}
};

class TestThread : public lwiot::Thread {
public:
	explicit TestThread(const char *name, const char *arg) : Thread(name, (void*)arg)
	{ }

protected:
	virtual void run(void *arg) override
	{
		int idx = 0;

		while(idx++ < 5) {
			print_dbg("Thread ping..\n");
			lwiot_sleep(750);
		}
	}
};

static TestThread *tp;
extern "C" void main_start(void)
{
	I2CTestApplication runner;
	lwiot::Application app(runner);

	tp = new TestThread("tst-1", nullptr);
	tp->start();
	app.start();
}

