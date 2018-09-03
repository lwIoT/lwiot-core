/*
 * DNS client header.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#ifndef LWIOT_DNS_H
#define LWIOT_DNS_H

#include <lwiot/types.h>

extern int hostname_to_ip(const char *host, uint32_t *addr);

#endif //LWIOT_DNS_H
