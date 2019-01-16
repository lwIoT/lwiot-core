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

CDECL

typedef struct PACKED_ATTR {
	uint8_t len;
	uint8_t data;
} DnsLabel;

typedef struct PACKED_ATTR {
	uint16_t type;
	uint16_t q_class;
} DnsQuestionFooter;

typedef struct PACKED_ATTR {
	uint16_t type;
	uint16_t r_class;
	uint32_t ttl;
	uint16_t rdlength;
} DnsResourceFooter;

typedef struct  PACKED_ATTR {
	uint16_t prio;
	uint16_t weight;
} DnsUriHdr;

typedef struct PACKED_ATTR {
	uint16_t id;
	uint8_t flags;
	uint8_t rcode;
	uint16_t qdcount;
	uint16_t ancount;
	uint16_t nscount;
	uint16_t arcount;
} DnsHeader;

#define QTYPE_A  1
#define QTYPE_NS 2
#define QTYPE_CNAME 5
#define QTYPE_SOA 6
#define QTYPE_WKS 11
#define QTYPE_PTR 12
#define QTYPE_HINFO 13
#define QTYPE_MINFO 14
#define QTYPE_MX 15
#define QTYPE_TXT 16
#define QTYPE_URI 256

#define QCLASS_IN 1
#define QCLASS_ANY 255
#define QCLASS_URI 256

#define DNS_SERVER_PORT 53
#define DNS_LEN 512

#define FLAG_QR (1<<7)
#define FLAG_AA (1<<2)
#define FLAG_TC (1<<1)
#define FLAG_RD (1<<0)

CDECL_END

#endif //LWIOT_DNS_H
