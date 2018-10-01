/*
 * AVR I2C implementation.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void avr_i2c_init(void);

extern uint8_t i2c_start(uint8_t address);
extern void i2c_stop(void);

extern uint8_t i2c_write(uint8_t data);
extern uint8_t i2c_read_ack(void);
extern uint8_t i2c_read_nack(void);

extern uint8_t i2c_transmit(uint8_t address, uint8_t* data, uint16_t length, uint8_t stop);
extern uint8_t i2c_receive(uint8_t address, uint8_t* data, uint16_t length, uint8_t stop);

extern uint8_t i2c_writeReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length);
extern uint8_t i2c_readReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length);

#ifdef __cplusplus
}
#endif
