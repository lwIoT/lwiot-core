/*
 * DNS wrapper implementation.
 *
 * @author Michel Megens
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <lwiot.h>

#include <lwiot/error.h>
#include <lwiot/types.h>
#include <lwiot/log.h>

#include <lwiot/network/stdnet.h>
#include <lwiot/network/dns.h>

#ifdef WIN32
#else

#include <errno.h>
#include <netdb.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#endif

typedef struct __attribute__ ((packed)) {
	uint8_t len;
	uint8_t data;
} DnsLabel;


typedef struct __attribute__ ((packed)) {
	uint16_t type;
	uint16_t class;
} DnsQuestionFooter;


typedef struct __attribute__ ((packed)) {
	uint16_t type;
	uint16_t class;
	uint32_t ttl;
	uint16_t rdlength;
} DnsResourceFooter;

typedef struct __attribute__ ((packed)) {
	uint16_t prio;
	uint16_t weight;
} DnsUriHdr;

typedef struct __attribute__ ((packed)) {
	uint16_t id;
	uint8_t flags;
	uint8_t rcode;
	uint16_t qdcount;
	uint16_t ancount;
	uint16_t nscount;
	uint16_t arcount;
} DnsHeader;


#define FLAG_QR (1<<7)
#define FLAG_AA (1<<2)
#define FLAG_TC (1<<1)
#define FLAG_RD (1<<0)


#define DNS_SERVER_PORT 53
#define DNS_LEN 512

static void dns_captive_receive(int sock_fd, uint32_t addr, struct sockaddr_in *premote_addr, char *pusrdata, unsigned short length)
{
	int i;
	char *buff = (char *) lwiot_mem_zalloc(DNS_LEN);
	char *reply = (char *) lwiot_mem_zalloc(DNS_LEN);
	char *rend = reply + length;
	char *p = pusrdata;
	uint8_t *ipaddr = (uint8_t*) &addr;

	DnsHeader *hdr = (DnsHeader *) p;
	DnsHeader *rhdr = (DnsHeader *) reply;
	p += sizeof(DnsHeader);

	if(length > 512)
		goto finish;
	if(length < sizeof(DnsHeader))
		goto finish;
	if(hdr->ancount || hdr->nscount || hdr->arcount)
		goto finish;
	if(hdr->flags & FLAG_TC)
		goto finish;

	memcpy(reply, pusrdata, length);
	rhdr->flags |= FLAG_QR;
	for(i = 0; i < local_ntohs(&hdr->qdcount); i++) {
		p = labelToStr(pusrdata, p, length, buff, DNS_LEN);
		if(p == NULL)
			goto finish;
		DnsQuestionFooter *qf = (DnsQuestionFooter *) p;
		p += sizeof(DnsQuestionFooter);
		print_dbg("DNS: Q (type 0x%X class 0x%X) for %s\n", local_ntohs(&qf->type), local_ntohs(&qf->class), buff);
		if(local_ntohs(&qf->type) == QTYPE_A) {
			rend = strToLabel(buff, rend, DNS_LEN - (rend - reply));
			if(rend == NULL)
				goto finish;
			DnsResourceFooter *rf = (DnsResourceFooter *) rend;
			rend += sizeof(DnsResourceFooter);
			setn16(&rf->type, QTYPE_A);
			setn16(&rf->class, QCLASS_IN);
			setn32(&rf->ttl, 0);
			setn16(&rf->rdlength, 4);
			*rend++ = ipaddr[0];
			*rend++ = ipaddr[1];
			*rend++ = ipaddr[2];
			*rend++ = ipaddr[3];
			setn16(&rhdr->ancount, local_ntohs(&rhdr->ancount) + 1);
		} else if(local_ntohs(&qf->type) == QTYPE_NS) {
			rend = strToLabel(buff, rend, DNS_LEN - (rend - reply)); //Add the label
			DnsResourceFooter *rf = (DnsResourceFooter *) rend;
			rend += sizeof(DnsResourceFooter);
			setn16(&rf->type, QTYPE_NS);
			setn16(&rf->class, QCLASS_IN);
			setn16(&rf->ttl, 0);
			setn16(&rf->rdlength, 4);
			*rend++ = 2;
			*rend++ = 'n';
			*rend++ = 's';
			*rend++ = 0;
			setn16(&rhdr->ancount, local_ntohs(&rhdr->ancount) + 1);
		} else if(local_ntohs(&qf->type) == QTYPE_URI) {
			rend = strToLabel(buff, rend, DNS_LEN - (rend - reply));
			DnsResourceFooter *rf = (DnsResourceFooter *) rend;
			rend += sizeof(DnsResourceFooter);
			DnsUriHdr *uh = (DnsUriHdr *) rend;
			rend += sizeof(DnsUriHdr);
			setn16(&rf->type, QTYPE_URI);
			setn16(&rf->class, QCLASS_URI);
			setn16(&rf->ttl, 0);
			setn16(&rf->rdlength, 4 + 16);
			setn16(&uh->prio, 10);
			setn16(&uh->weight, 1);
			memcpy(rend, "http://lwiot.net", 16);
			rend += 16;
			setn16(&rhdr->ancount, local_ntohs(&rhdr->ancount) + 1);
		}
	}

	sendto(sock_fd, (uint8_t *) reply, rend - reply, 0, (struct sockaddr *) premote_addr, sizeof(struct sockaddr_in));

finish:
	if(buff) {
		free(buff);
	}

	if(reply) {
		free(reply);
	}
}

static void captive_portal_task(void *argument)
{
	struct sockaddr_in server_addr;
	int32_t ret;
	int sock_fd;
	uint32_t portal_addr;
	struct sockaddr_in from;
	socklen_t fromlen;
	int nNetTimeout = 10000;
	char *udp_msg = (char *) lwiot_mem_zalloc(DNS_LEN);

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(DNS_SERVER_PORT);

#ifdef HAVE_LWIP
	server_addr.sin_len = sizeof(server_addr);
#endif

	portal_addr = *(uint32_t*)argument;

	do {
		sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
		if(sock_fd == -1) {
			printf("Captdns_task failed to create sock!\n");
			lwiot_sleep(1000);
		}
	} while(sock_fd == -1);

	do {
		ret = bind(sock_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
		if(ret != 0) {
			printf("Captdns_task failed to bind sock!\n");
			lwiot_sleep(1000);
		}
	} while(ret != 0);

	while(1) {

		memset(udp_msg, 0, DNS_LEN);
		memset(&from, 0, sizeof(from));

		setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, (char *) &nNetTimeout, sizeof(int));
		fromlen = sizeof(struct sockaddr_in);
		ret = recvfrom(sock_fd, (uint8_t *) udp_msg, DNS_LEN, 0, (struct sockaddr *) &from, (socklen_t *) &fromlen);
		if(ret > 0)
			dns_captive_receive(sock_fd, portal_addr, &from, udp_msg, ret);
	}
}

static lwiot_thread_t *tp;
void dns_start_captive_portal(uint32_t *addr)
{
	const char *name = "capt-dns";

	tp = lwiot_thread_create(captive_portal_task, name, addr);
	//lwiot_thread_create(&tp, captive_portal_task, addr);
}

void dns_stop_captive_portal(void)
{
	if(!tp)
		return;

	lwiot_thread_destroy(tp);
}
