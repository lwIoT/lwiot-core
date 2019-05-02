/*
 * AVR I2C implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <lwiot.h>

#include <lwiot/log.h>
#include <lwiot/types.h>

#include "i2c.h"

uint8_t i2c_write_start(uint8_t address)
{
	print_dbg("I2C start\n");
	return true;
}

uint8_t i2c_write(uint8_t data)
{
	print_dbg("I2C write byte: %u\n", data);
	return false;
}

uint8_t i2c_read_ack(void)
{
	print_dbg("I2C read byte. ACK expected!\n");
	return 0xAA;
}

uint8_t i2c_read_nack(void)
{
	print_dbg("I2C read byte. NACK expected!\n");
	return 0xAB;
}

uint8_t i2c_read(bool ack)
{
	uint8_t byte;

	if(ack)
		byte = i2c_read_ack();
	else
		byte = i2c_read_nack();

	return byte;
}

void i2c_write_stop(void)
{
	print_dbg("I2C stop issued!\n");
}