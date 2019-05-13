/*
 * DNS server implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <lwiot.h>

#include <lwiot/network/stdnet.h>
#include <lwiot/network/udpclient.h>
#include <lwiot/network/udpserver.h>
#include <lwiot/network/dnsserver.h>

#include <lwiot/stl/move.h>
#include <lwiot/bytebuffer.h>

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

static char *str_to_label(char *str, char *label, int maxLen)
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

	DnsServer::DnsServer() : Thread("dns-server")
	{
		this->_udp_msg = (char*) lwiot_mem_zalloc(DNS_LEN);
	}

	DnsServer::~DnsServer()
	{
		this->_udp->close();
		lwiot_mem_free(this->_udp_msg);
	}

	void DnsServer::end()
	{
		ScopedLock lock(this->_lock);

		this->_running = false;
		lock.unlock();
		this->stop();
		this->_udp->close();
	}

	void DnsServer::begin()
	{
		ScopedLock lock(this->_lock);

		this->_running = true;
		this->start();
	}

	void DnsServer::begin(lwiot::UdpServer *server)
	{
		ScopedLock lock(this->_lock);

		this->_udp.reset(server);
		lock.unlock(); /* Prevent recursion, as some systems may not support recursive locks */
		this->begin();
	}

	void DnsServer::map(const lwiot::String &hostname, const lwiot::IPAddress &addr)
	{
		this->_table.add(hostname, addr);
	}

	void DnsServer::run()
	{
		size_t num;
		bool running_;

		this->_lock.lock();
		running_ = this->_running;
		this->_lock.unlock();

		while(running_) {
			Thread::yield();
			ScopedLock lock(this->_lock);
			memset(this->_udp_msg, 0, DNS_LEN);
			num = DNS_LEN;

			this->_udp->setTimeout(1);
			auto client = this->_udp->recv(this->_udp_msg, num);

			if(client)
				this->respond(*client, this->_udp_msg, num);

			running_ = this->_running;
		}
	}

	void DnsServer::respond(lwiot::UdpClient &client, DnsHeader *hdr, lwiot::DnsReplyCode drc)
	{
		hdr->rcode = static_cast<uint8_t>(drc);
		hdr->flags |= FLAG_QR;

		hdr->ancount = 0;
		hdr->arcount = 0;
		hdr->nscount = 0;
		hdr->qdcount = 0;

		client.write(hdr, sizeof(*hdr));
	}

	void DnsServer::respond(UdpClient &client, char *data, const size_t &length)
	{
		int i;
		uint32_t addr;
		uint8_t *ipaddr;
		lwiot::ByteBuffer replybuffer(DNS_LEN);
		lwiot::ByteBuffer buffer(DNS_LEN);

		char *rawbuffer = (char *) buffer.data();
		char *rawreply = (char *) replybuffer.data();
		char *rend = rawreply + length;
		char *p = data;

		ipaddr = (uint8_t*)&addr;

		DnsHeader *hdr = (DnsHeader *) p;
		DnsHeader *rhdr = (DnsHeader *) rawreply;
		p += sizeof(DnsHeader);

		if(length > DNS_LEN)
			return;

		if(length < sizeof(DnsHeader))
			return;

		if(hdr->ancount || hdr->nscount || hdr->arcount)
			return;

		if(hdr->flags & FLAG_TC)
			return;

		memcpy(rawreply, data, length);
		rhdr->flags |= FLAG_QR;

		for(i = 0; i < local_ntohs(&hdr->qdcount); i++) {
			DnsQuestionFooter *qf;

			p = label_to_str(data, p, length, rawbuffer, DNS_LEN);
			if(p == nullptr)
				return;

			qf = (DnsQuestionFooter *) p;
			p += sizeof(DnsQuestionFooter);
			print_dbg("DNS: Q (type 0x%X class 0x%X) for %s\n",
					local_ntohs(&qf->type), local_ntohs(&qf->q_class), rawbuffer);
			stl::String record(rawbuffer);

			if(local_ntohs(&qf->type) == QTYPE_A) {
				DnsResourceFooter *rf;

				if(!this->_table.contains(record)) {
					DnsServer::respond(client, hdr, DnsReplyCode::NonExistentDomain);
					return;
				}

				addr = this->_table.at(record);
				rend = str_to_label(rawbuffer, rend, DNS_LEN - (rend - rawreply));
				rf = (DnsResourceFooter *) rend;

				if(rend == NULL)
					return;

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
				DnsResourceFooter *rf;

				rend = str_to_label(rawbuffer, rend, DNS_LEN - (rend - rawreply)); //Add the label
				rf = (DnsResourceFooter *) rend;
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
				DnsResourceFooter *rf;
				DnsUriHdr *uh;

				rend = str_to_label(rawbuffer, rend, DNS_LEN - (rend - rawreply));
				rf = (DnsResourceFooter *) rend;
				rend += sizeof(DnsResourceFooter);
				uh = (DnsUriHdr *) rend;
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

		client.write(rawreply, static_cast<size_t>(rend - rawreply));
	}
}
