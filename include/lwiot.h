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
extern "C" void lwiot_destroy();

#ifndef NO_OS
extern lwiot_event_t* lwiot_dns_event;
#endif

#else /* __cplusplus */

extern void lwiot_init();
extern void lwiot_destroy();

#ifndef NO_OS
extern lwiot_event_t* lwiot_dns_event;
#endif
#endif

