/*
 * Captive portal implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/error.h>
#include <lwiot/scopedlock.h>

#include <lwiot/network/udpclient.h>
#include <lwiot/network/udpserver.h>
#include <lwiot/network/captiveportal.h>
#include <lwiot/stl/move.h>

static void setn16(void *pp, int16_t n)
{
	char *p = (char*)pp;

	*p++ = (n >> 8);
	*p = (n & 0xff);
}

static void setn32(void *pp, int32_t n)
{
	char *p = (char*)pp;

	*p++ = (n >> 24) & 0xff;
	*p++ = (n >> 16) & 0xff;
	*p++ = (n >> 8) & 0xff;
	*p = (n & 0xff);
}


static uint16_t local_ntohs(uint16_t *in)
{
	char *p = (char *) in;
	return ((p[0] << 8) & 0xff00) | (p[1] & 0xff);
}

static char *label_to_str(char *packet, char *labelPtr, int packetSz, char *res, int resMaxLen)
{
	int i, j, k;
	char *endPtr = NULL;
	i = 0;
	do {
		if((*labelPtr & 0xC0) == 0) {
			j = *labelPtr++;
			if(i < resMaxLen && i != 0)
				res[i++] = '.';

			for(k = 0; k < j; k++) {
				if((labelPtr - packet) > packetSz)
					return NULL;
				if(i < resMaxLen)
					res[i++] = *labelPtr++;
			}
		} else if((*labelPtr & 0xC0) == 0xC0) {
			endPtr = labelPtr + 2;
			int offset = local_ntohs(((uint16_t *) labelPtr)) & 0x3FFF;
			if(offset > packetSz)
				return NULL;

			labelPtr = &packet[offset];
		}

		if((labelPtr - packet) > packetSz)
			return NULL;
	} while(*labelPtr != 0);

	res[i] = 0;
	if(endPtr == NULL)
		endPtr = labelPtr + 1;
	return endPtr;
}

static char *strToLabel(char *str, char *label, int maxLen)
{
	char *len = label;
	char *p = label + 1;
	while(1) {
		if(*str == '.' || *str == 0) {
			*len = ((p - len) - 1);
			len = p;
			p++;
			if(*str == 0)
				break;
			str++;
		} else {
			*p++ = *str++;
		}
	}
	*len = 0;
	return p;
}

namespace lwiot
{
	CaptivePortal::CaptivePortal(const IPAddress& addr, const IPAddress& captor, uint16_t port, UdpServer* server) :
		Thread("cp", nullptr), _lock(false), _udp(server), _captor(captor), _running(false), _bind_addr(addr), _port(port)
	{
		this->udp_msg = (char*) lwiot_mem_zalloc(DNS_LEN);
	}

	CaptivePortal::~CaptivePortal()
	{
		this->_udp->close();
		lwiot_mem_free(this->udp_msg);
	}

	void CaptivePortal::end()
	{
		ScopedLock lock(this->_lock);

		this->_running = false;
		lock.unlock();
		this->stop();
		this->_udp->close();
	}

	void CaptivePortal::begin()
	{
		ScopedLock lock(this->_lock);

#ifndef NDEBUG
		auto value = this->_udp->bind(this->_bind_addr, this->_port);
		assert(value);
#else
		this->_udp->bind(this->_bind_addr, this->_port);
#endif
		this->_running = true;
		this->start();
	}

	void CaptivePortal::begin(lwiot::UdpServer *server, uint16_t port)
	{
		ScopedLock lock(this->_lock);

		this->_udp.reset(server);
		this->_port = port;
		lock.unlock(); /* Prevent recursion, as some systems may not support recursive locks */
		this->begin();
	}

	void CaptivePortal::respond(lwiot::UdpClient &client, char *data, const size_t &length)
	{
		int i;
		char *buff = (char *) lwiot_mem_zalloc(DNS_LEN);
		char *reply = (char *) lwiot_mem_zalloc(DNS_LEN);
		char *rend = reply + length;
		char *p = data;
		uint32_t addr;
		uint8_t *ipaddr;

		addr = this->_captor;
		ipaddr = (uint8_t*)&addr;

		DnsHeader *hdr = (DnsHeader *) p;
		DnsHeader *rhdr = (DnsHeader *) reply;
		p += sizeof(DnsHeader);

		if(length > DNS_LEN)
			goto finish;

		if(length < sizeof(DnsHeader))
			goto finish;

		if(hdr->ancount || hdr->nscount || hdr->arcount)
			goto finish;

		if(hdr->flags & FLAG_TC)
			goto finish;

		memcpy(reply, data, length);
		rhdr->flags |= FLAG_QR;
		for(i = 0; i < local_ntohs(&hdr->qdcount); i++) {
			p = label_to_str(data, p, length, buff, DNS_LEN);
			if(p == nullptr)
				goto finish;

			DnsQuestionFooter *qf = (DnsQuestionFooter *) p;
			p += sizeof(DnsQuestionFooter);
			print_dbg("DNS: Q (type 0x%X class 0x%X) for %s\n", local_ntohs(&qf->type), local_ntohs(&qf->q_class), buff);
			if(local_ntohs(&qf->type) == QTYPE_A) {
				rend = strToLabel(buff, rend, DNS_LEN - (rend - reply));
				if(rend == NULL)
					goto finish;
				DnsResourceFooter *rf = (DnsResourceFooter *) rend;
				rend += sizeof(DnsResourceFooter);
				setn16(&rf->type, QTYPE_A);
				setn16(&rf->r_class, QCLASS_IN);
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
				setn16(&rf->r_class, QCLASS_IN);
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
				setn16(&rf->r_class, QCLASS_URI);
				setn16(&rf->ttl, 0);
				setn16(&rf->rdlength, 4 + 16);
				setn16(&uh->prio, 10);
				setn16(&uh->weight, 1);
				memcpy(rend, "http://lwiot.net", 16);
				rend += 16;
				setn16(&rhdr->ancount, local_ntohs(&rhdr->ancount) + 1);
			}
		}

		client.write(reply, static_cast<size_t>(rend - reply));

finish:
		if(buff != nullptr)
			lwiot_mem_free(buff);

		if(reply != nullptr)
			lwiot_mem_free(reply);
	}

	void CaptivePortal::run()
	{
		size_t num;
		bool running;

		this->_lock.lock();
		running = this->_running;
		this->_lock.unlock();

		while(running) {
			this->yield();
			ScopedLock lock(this->_lock);
			memset(udp_msg, 0, DNS_LEN);
			num = DNS_LEN;

			this->_udp->setTimeout(10000);
			auto client = this->_udp->recv(udp_msg, num);

			if(client)
				this->respond(*client, udp_msg, num);

			running = this->_running;
		}
	}
}
