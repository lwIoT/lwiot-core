/*
 * TCP server wrapper
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/log.h>
#include <lwiot/string.h>
#include <lwiot/stream.h>
#include <lwiot/network/ipaddress.h>
#include <lwiot/network/tcpclient.h>
#include <lwiot/network/stdnet.h>
#include <lwiot/network/tcpserver.h>
#include <lwiot/uniquepointer.h>

namespace lwiot
{
	class SocketTcpServer : TcpServer {
	public:
		explicit SocketTcpServer();
		explicit SocketTcpServer(const IPAddress& addr, uint16_t port);
		~SocketTcpServer() override;

		SocketTcpServer& operator =(const SocketTcpServer& other);

		bool operator ==(const SocketTcpServer& other);
		bool operator !=(const SocketTcpServer& other);

		bool bind(const IPAddress& addr, uint16_t port) override;
		bool bind() const override;

		void connect() override;
		UniquePointer<TcpClient> accept() override;
		void close() override;

#ifdef HAVE_LWIP
		static constexpr int BacklogSize = 2;
#else
		static constexpr int BacklogSize = 128;
#endif

	private:
		socket_t *_socket;
	};
}
