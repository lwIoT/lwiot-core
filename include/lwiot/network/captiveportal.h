/*
 * Captive portal header.
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
#include <lwiot/thread.h>
#include <lwiot/lock.h>

#include <lwiot/network/udpclient.h>
#include <lwiot/network/udpserver.h>
#include <lwiot/network/dns.h>

namespace lwiot
{
	class CaptivePortal : Thread {
	public:
		explicit CaptivePortal(const IPAddress& captor, uint16_t port = DNS_SERVER_PORT, UdpServer* server = nullptr);
		virtual ~CaptivePortal();

		void begin();
		void begin(UdpServer* server, uint16_t port);
		void end();

	protected:
		void run(void *arg) override;

	private:
		Lock _lock;
		UniquePointer<UdpServer> _udp;
		IPAddress _captor;
		bool _running;
		uint16_t _port;
		char *udp_msg;

		/* Methods */
		void respond(UdpClient& client, char *data, const size_t& length);
	};
}
