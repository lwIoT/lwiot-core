/*
 * HAL for I2C.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <stdint.h>

#include <lwiot.h>
#include <lwiot/types.h>

CDECL

extern void i2c_setup(int scl, int sda, uint32_t freq);
extern void i2c_set_frequency(uint32_t freq);
extern void i2c_reset();
extern void i2c_disable();

extern void i2c_start(uint16_t sla, bool repeated);
extern void i2c_stop();
extern void i2c_write_byte(const uint8_t *byte, bool ack);
extern void i2c_write_buffer(const uint8_t *bytes, size_t length, bool ack);
extern void i2c_read_byte(uint8_t* byte, bool ack);
extern void i2c_read_buffer(uint8_t* bytes, size_t length, bool ack);
extern int i2c_write_buffers();

CDECL_END
