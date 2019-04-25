/*
 * UDP server wrapper class.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/network/stdnet.h>
#include <lwiot/network/udpclient.h>
#include <lwiot/network/udpserver.h>
#include <lwiot/network/socketudpserver.h>

namespace lwiot
{
	class SocketUdpServer : public UdpServer {
	public:
		explicit SocketUdpServer();
		explicit SocketUdpServer(bind_addr_t addr, uint16_t port);
		explicit SocketUdpServer(const IPAddress& addr, uint16_t port);
		~SocketUdpServer() override;

		void close() override;
		bool bind() override;
		bool bind(bind_addr_t addr, uint16_t port);
		bool bind(const IPAddress& addr, uint16_t port) override;

		UniquePointer<UdpClient> recv(void *buffer, size_t& length) override;
		void setTimeout(int tmo) override;

	private:
		socket_t* _socket;
	};
}
