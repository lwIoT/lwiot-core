/*
 * DNS server implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file dnsserver.h

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
	/**
	 * @brief DNS server implementation.
	 * @see Thread
	 *
	 * This is an assynchronous implementation.
	 */
	class DnsServer : public Thread {
	public:
		/**
		 * @brief DNS server implementation.
		 */
		explicit DnsServer();

		/**
		 * @brief DNS server destructor.
		 */
		~DnsServer() override;

		/**
		 * @brief Start a DNS server.
		 * @param server UDP server object.
		 */
		void begin(UdpServer* server);

		/**
		 * @brief Stop a DNS server.
		 */
		void end();

		/**
		 * @brief Map a hostname to an IP address.
		 * @param hostname Hostname to map to \p addr.
		 * @param addr IP address to map to \p hostname.
		 */
		void map(const stl::String& hostname, const IPAddress& addr);

	protected:
		/**
		 * @brief Run the DNS server.
		 * @see Thread::run
		 */
		void run() override;

		/**
		 * @brief Start the DNS server.
		 */
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
	};
}
