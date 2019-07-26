/*
 * TCP client wrapper.
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
#include <lwiot/stl/string.h>
#include <lwiot/stream.h>
#include <lwiot/network/ipaddress.h>
#include <lwiot/network/stdnet.h>
#include <lwiot/network/tcpclient.h>

namespace lwiot
{
	class SocketTcpClient : public TcpClient {
	public:
		explicit SocketTcpClient();
		explicit SocketTcpClient(socket_t* raw);
		explicit SocketTcpClient(const IPAddress& addr, uint16_t port);
		explicit SocketTcpClient(const String& host, uint16_t port);
		explicit SocketTcpClient(const SocketTcpClient& other) ;
		explicit SocketTcpClient(SocketTcpClient&& other) noexcept ;
		~SocketTcpClient() override;

		SocketTcpClient& operator =(const SocketTcpClient& client);
		SocketTcpClient& operator =( SocketTcpClient&& client) noexcept ;

		bool operator ==(const SocketTcpClient& other);
		bool operator !=(const SocketTcpClient& other);

		explicit operator bool() const override;
		bool connected() const override;

		size_t available() const override;

		using TcpClient::read;
		using TcpClient::write;

		ssize_t read(void *output, const size_t &length) override;
		ssize_t write(const void *bytes, const size_t& length) override;

		bool connect(const IPAddress& addr, uint16_t port) override;
		bool connect(const String& host, uint16_t port) override;
		void setTimeout(time_t seconds) override;

		void close() override;

	private:
		socket_t *_socket;

		bool connect();
	};
}
