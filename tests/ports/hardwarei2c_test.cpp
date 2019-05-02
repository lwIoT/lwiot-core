/*
 * Unit test for hardware I2C.
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
#include <lwiot/kernel/thread.h>

#include <lwiot/io/i2cbus.h>
#include <lwiot/io/hardwarei2calgorithm.h>
#include <lwiot/io/gpiopin.h>

#include <lwiot/log.h>
#include <lwiot/test.h>

#ifdef NDEBUG
#error "Debugging not enabled.."
#endif

class ThreadTest : public lwiot::Thread {
public:
	explicit ThreadTest(const char *name) : Thread(name, nullptr)
	{
	}

protected:
	static void testRead(lwiot::I2CBus& bus)
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

	static void testSingle(lwiot::I2CBus& bus)
	{
		lwiot::I2CMessage wr(1);

		wr.setAddress(0x6B, false, false);
		wr.write(1);
		wr.setRepeatedStart(false);

		if(bus.transfer(wr)) {
			print_dbg("Single test successfull!\n");
		} else {
			print_dbg("Single test failed!\n");
		}

		lwiot_sleep(500);
	}

	void run() override
	{
		lwiot::GpioPin scl(1), sda(2);
		lwiot::HardwareI2CAlgorithm *algo = new lwiot::HardwareI2CAlgorithm(scl, sda, 400000);
		lwiot::I2CBus bus(algo);

		print_dbg("---------- Starting MOVE test ----------\n");
		lwiot::HardwareI2CAlgorithm a1(5, 6, 100000);
		lwiot::HardwareI2CAlgorithm a2(3, 4, 100000);
		a1 = lwiot::stl::move(a2);
		print_dbg("---------- End of MOVE test ----------\n");

		print_dbg("---------- Starting I2C test... ----------\n");
		ThreadTest::testRead(bus);
		print_dbg("---------- End of I2C test... ----------\n");
		print_dbg("---------- Starting I2C test... ----------\n");
		ThreadTest::testSingle(bus);
		print_dbg("---------- End of I2C test... ----------\n");

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
	t1.stop();
	lwiot_destroy();

	return -EXIT_SUCCESS;
}
