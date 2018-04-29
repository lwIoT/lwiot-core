/*
 * lwIoT initialisation header.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/lwiot.h>

#ifdef __cplusplus
extern "C" void lwiot_init();
#else
extern void lwiot_init();
#endif

