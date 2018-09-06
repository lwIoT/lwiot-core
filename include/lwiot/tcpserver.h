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
#include <lwiot/ipaddress.h>
#include <lwiot/tcpclient.h>

#ifdef WIN32
#else
#ifdef HAVE_LWIP
extern "C" {
#include <lwip/sys.h>
#include <lwip/sockets.h>
};
#else
#include <netinet/in.h>
#endif
#endif

namespace lwiot
{
	class TcpServer {
	public:
		explicit TcpServer();
		explicit TcpServer(const IPAddress& addr, uint16_t port);
		virtual ~TcpServer();

		TcpServer& operator =(const TcpServer& other);

		bool operator ==(const TcpServer& other);
		bool operator !=(const TcpServer& other);

		bool bind(const IPAddress& addr, uint16_t port);
		bool bind() const;

		TcpClient accept() const;
		void close();

#ifdef HAVE_LWIP
		static constexpr int BacklogSize = 2;
#else
		static constexpr int BacklogSize = 128;
#endif

	private:
		IPAddress _bind_addr;
		uint16_t _bind_port;
		int _fd;
	};
}
