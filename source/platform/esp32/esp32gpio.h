/*
 * ESP32 GPIO definitions.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#define RAW_INPUT 1
#define RAW_OUTPUT 2
#define RAW_OUTPUT_OPENDRAIN 3

#ifdef __cplusplus
extern "C" void pinMode(int pin, int mode);
extern "C" void digitalWrite(int pin, bool value);
#else
extern void pinMode(int pin, int mode);
extern void digitalWrite(int pin, bool value);
#endif
