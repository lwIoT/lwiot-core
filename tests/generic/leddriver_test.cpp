/*
 * Logging unit test.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/log.h>

#include <lwiot/io/pwm.h>
#include <lwiot/io/rgbleddriver.h>

#include <lwiot/test.h>

class TestPwmChannel : public lwiot::PwmChannel {
public:
	virtual void enable() { }
	virtual void disable() { }

	virtual void reload()
	{
		print_dbg("Duty cycle: %f\n", this->duty());
	}

protected:
	virtual void update(int freq) { }
};

int main(int argc, char **argv)
{
	lwiot_init();
	print_dbg("Starting PWM test!\n");
	TestPwmChannel r,g,b;
	lwiot::RgbLedDriver rgb(r, g, b);

	rgb.set(10, 10, 10);

	print_dbg("Fading to RGB value..");
	rgb.fade(127, 66, 168);

	print_dbg("Fading to RGB value..");
	rgb.fade(250, 12, 30);


	lwiot_destroy();
	wait_close();
	return -EXIT_SUCCESS;
}
