/*
 * I2C transmission data container.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/log.h>
#include <lwiot/error.h>
#include <lwiot/types.h>
#include <lwiot/i2cmessage.h>
#include <lwiot/test.h>

int main(int argc, char **argv)
{
	lwiot_init();

	lwiot::I2CMessage msg(5);
	lwiot::Logger log;

	msg.setAddress(0x5E, false, false);
	msg.write(0xAA);
	msg.write(0xAB);
	msg.write(0xAC);
	msg.write(0xAD);
	msg.write(0xFA);

	log << "Message size: " << msg.count() << lwiot::Logger::newline;
	auto printer = [&log](uint8_t byte) -> void {
		log << byte;
		log << " ";
	};

	msg.foreach(printer);
	log << lwiot::Logger::newline;

	lwiot_destroy();
	wait_close();
}
