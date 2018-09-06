/*
 * DNS client header.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#ifndef LWIOT_DNS_H
#define LWIOT_DNS_H

#include <lwiot/lwiot.h>
#include <lwiot/types.h>

extern DLL_EXPORT void dns_start_captive_portal(uint32_t *addr);
extern DLL_EXPORT int hostname_to_ip(const char *host, uint32_t *addr);

#endif //LWIOT_DNS_H
