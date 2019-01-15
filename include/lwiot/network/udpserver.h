/*
 * UDP server wrapper class.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/network/ipaddress.h>
#include <lwiot/uniquepointer.h>

namespace lwiot
{
	class UdpServer {
	public:
		explicit UdpServer();
		explicit UdpServer(const IPAddress& addr, uint16_t port);
		virtual ~UdpServer() = default;

		virtual void close() = 0;
		virtual bool bind() = 0;
		virtual bool bind(bind_addr_t addr, uint16_t port) = 0;
		virtual bool bind(const IPAddress& addr, uint16_t port);

		virtual UniquePointer<UdpClient> recv(void *buffer, size_t& length) = 0;
		virtual void setTimeout(int tmo) = 0;

		const IPAddress& address() const;
		uint16_t port() const;

	private:
		IPAddress _bind_addr;
		uint16_t _bind_port;
	};
}
