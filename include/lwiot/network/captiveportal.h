/*
 * Captive portal header.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file captiveportal.h

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

namespace lwiot
{
	/**
	 * @brief Captive portal.
	 * @see Thread
	 */
	class CaptivePortal : Thread {
	public:
		/**
		 * @brief Construct a new captive portal object.
		 * @param bind Address to bind to.
		 * @param captor Captor address.
		 * @param port DNS port.
		 * @param server DNS server.
		 */
		explicit CaptivePortal(const IPAddress& bind, const IPAddress& captor, uint16_t port = DNS_SERVER_PORT, UdpServer* server = nullptr);

		/**
		 * @brief Destroy a new captive portal object.
		 */
		~CaptivePortal() override;

		/**
		 * @brief Start a captive portal.
		 */
		void begin();

		/**
		 * @brief Start a captive portal.
		 * @param server UDP server object.
		 * @param port UDP server port.
		 */
		void begin(UdpServer* server, uint16_t port);

		/**
		 * @brief Stop the captive portal.
		 */
		void end();

	protected:
		/**
		 * @brief Captive portal runner.
		 * @see Thread::run
		 */
		void run() override;

	private:
		Lock _lock;
		UniquePointer<UdpServer> _udp;
		IPAddress _captor;
		bool _running;
		IPAddress _bind_addr;
		uint16_t _port;
		char *udp_msg;

		/* Methods */
		void respond(UdpClient& client, char *data, const size_t& length);
	};
}
