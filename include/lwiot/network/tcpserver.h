/*
 * TCP server wrapper
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/log.h>
#include <lwiot/string.h>
#include <lwiot/stream.h>
#include <lwiot/network/ipaddress.h>
#include <lwiot/network/tcpclient.h>
#include <lwiot/uniquepointer.h>

namespace lwiot
{
	class TcpServer {
	public:
		explicit TcpServer();
		explicit TcpServer(const IPAddress& addr, uint16_t port);
		virtual ~TcpServer() = default;

		TcpServer& operator =(const TcpServer& other);

		bool operator ==(const TcpServer& other);
		bool operator !=(const TcpServer& other);

		virtual bool bind(const IPAddress& addr, uint16_t port) = 0;
		virtual bool bind() const = 0;

		virtual void connect() = 0;

		virtual UniquePointer<TcpClient> accept() = 0;
		virtual void close() = 0;

		const IPAddress& address() const { return this->_bind_addr; }
		uint16_t port() const { return this->_bind_port; }

#ifdef HAVE_LWIP
		static constexpr int BacklogSize = 4;
#else
		static constexpr int BacklogSize = 128;
#endif

	protected:
		IPAddress _bind_addr;
		uint16_t _bind_port;
	};
}
