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

#include <lwiot/kernel/thread.h>
#include <lwiot/kernel/functionalthread.h>

#include <lwiot/log.h>
#include <lwiot/io/gpiochip.h>
#include <lwiot/io/gpiopin.h>
#include <lwiot/io/watchdog.h>
#include <lwiot/io/i2cbus.h>
#include <lwiot/io/gpioi2calgorithm.h>
#include <lwiot/io/i2cmessage.h>
#include <lwiot/device/apds9301sensor.h>

#include <lwiot/util/datetime.h>
#include <lwiot/util/application.h>

#include <lwiot/stl/vector.h>
#include <lwiot/stl/string.h>

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
private:
	lwiot::Apds9301Sensor *_sensor;

protected:
	void run() override
	{
		size_t freesize;
		//auto algo = new lwiot::GpioI2CAlgorithm(23, 22, 100000U);
		lwiot::I2CBus bus(new lwiot::esp32::I2CAlgorithm(23, 22, 400000U));
		this->_sensor = new lwiot::Apds9301Sensor(bus);

		printf("Main thread started!\n");
		//algo->test();
		lwiot_sleep(1000);

		lwiot::DateTime dt;
		print_dbg("Time: %s\n", dt.toString().c_str());
		freesize = heap_caps_get_free_size(0);
		wdt.enable();

		print_dbg("Free heap size: %u\n", freesize);
		print_dbg("GPIO I2C udelay value: %i\n", calculate_udelay_test(100000UL));

		print_dbg("Testing I2C bus..\n");

		lwiot::Function<void(void)> func([]() -> void {
			int idx = 0;

			while(idx++ < 5) {
				print_dbg("[ftp] Thread ping..\n");
				lwiot_sleep(750);
			}
		});

		lwiot::FunctionalThread td("functp", func);
		td.start();


		while(true) {
			//this->_sensor->getLux(luxdata);

			lwiot::stl::Vector<lwiot::I2CMessage*> msgs;
			lwiot::I2CMessage wr(1);
			lwiot::I2CMessage rd(32);
			lwiot::I2CMessage rd2(32);

			wr.setRepeatedStart(true);
			wr.setAddress(0x6B, false, false);
			wr.write(2);
			msgs.pushback(&wr);

			rd.setAddress(0x6B, false, false);
			rd.setRepeatedStart(true);
			rd2.setAddress(0x6B, false, false);
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


			lwiot_sleep(200);
			wdt.reset();

			this->testSingle(bus);
			lwiot_sleep(150);

			this->testRead(bus);

			wdt.reset();
			lwiot_sleep(500);
		}
	}

private:
	void testSingle(lwiot::I2CBus& bus)
	{
		lwiot::I2CMessage wr(1), rd(3);
		lwiot::stl::Vector<lwiot::I2CMessage*> msgs(2);

		wr.setAddress(0x6B, false, false);
		wr.write(1);
		wr.setRepeatedStart(true);

		rd.setAddress(0x6B, false, true);
		rd.setRepeatedStart(false);

		msgs.pushback(&wr);
		msgs.pushback(&rd);

		if(bus.transfer(msgs)) {
			auto b1 = rd[0];
			auto b2 = rd[1];
			auto b3 = rd[2];

			assert(b2 == (b1 + 1));
			assert(b3 == b2 + 1);
			print_dbg("Single test successfull!\n");
		} else {
			print_dbg("Single test failed!\n");
		}

		lwiot_sleep(500);
	}

	void testRead(lwiot::I2CBus& bus)
	{
		lwiot::I2CMessage wr(1), rd(3);
		lwiot::stl::Vector<lwiot::I2CMessage*> msgs;

		wr.setAddress(0x6B, false, false);
		wr.write(1);
		wr.setRepeatedStart(true);

		rd.setAddress(0x6B, false, true);
		rd.setRepeatedStart( false);

		msgs.pushback(&wr);
		msgs.pushback(&rd);

		if(bus.transfer(msgs)) {
			print_dbg("Read test successfull!\n");
		} else {
			print_dbg("Read test failed!\n");
		}

		auto& msg = *msgs[1];
		print_dbg("Read data:\n");
		print_dbg("Read byte: %u\n", msg[0]);
		print_dbg("Read byte: %u\n", msg[1]);
		print_dbg("Read byte: %u\n", msg[2]);

		lwiot_sleep(500);
	}
};

class TestThread : public lwiot::Thread {
public:
	explicit TestThread(const char *name, const char *arg) : Thread(name, 4, 2048, (void*)arg)
	{ }

protected:
	void run() override
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
