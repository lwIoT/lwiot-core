/*
 * DNS server implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/error.h>
#include <lwiot/uniquepointer.h>

#include <lwiot/kernel/thread.h>
#include <lwiot/kernel/lock.h>

#include <lwiot/network/stdnet.h>
#include <lwiot/network/udpclient.h>
#include <lwiot/network/udpserver.h>
#include <lwiot/network/dns.h>

#include <lwiot/stl/map.h>

namespace lwiot
{
	class DnsServer : public Thread {
	public:
		explicit DnsServer();
		virtual ~DnsServer();

		void begin(UdpServer* server);
		void end();

		void map(const stl::String& hostname, const IPAddress& addr);

	protected:
		void run() override;
		void begin();

	private:
		Lock _lock;
		UniquePointer<UdpServer> _udp;
		stl::Map<stl::String, IPAddress> _table;
		bool _running;
		char *_udp_msg;

		/* Methods */
		void respond(UdpClient& client, char *data, const size_t& length);
		static void respond(UdpClient& client, DnsHeader* hdr, DnsReplyCode drc);
		bool hasRecord(const String& record);
	};
}
