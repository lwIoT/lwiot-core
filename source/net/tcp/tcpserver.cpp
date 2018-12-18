/*
 * TCP server wrapper
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/log.h>
#include <lwiot/error.h>
#include <lwiot/string.h>
#include <lwiot/stream.h>

#include <lwiot/network/ipaddress.h>
#include <lwiot/network/tcpserver.h>

#if !defined(HAVE_LWIP) && !defined(WIN32)
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

extern "C" {
#include "lwiot/network/dns.h"
}

namespace lwiot
{
	TcpServer::TcpServer() : _bind_addr(0U), _bind_port(0)
	{
	}

	TcpServer::TcpServer(const lwiot::IPAddress &addr, uint16_t port) : _bind_addr(addr), _bind_port(to_netorders(port))
	{
	}

	TcpServer& TcpServer::operator=(const lwiot::TcpServer &server)
	{
		this->_bind_addr = server._bind_addr;
		this->_bind_port = server._bind_port;
		this->bind();

		return *this;
	}

	bool TcpServer::operator==(const lwiot::TcpServer &other)
	{
		auto addr = this->_bind_addr == other._bind_addr;
		auto port = this->_bind_port == other._bind_port;

		return addr && port;
	}

	bool TcpServer::bind(const lwiot::IPAddress &addr, uint16_t port)
	{
		this->_bind_addr = addr;
		this->_bind_port = port;

		return true;
	}

	bool TcpServer::operator!=(const lwiot::TcpServer &other)
	{
		return !(*this == other);
	}
}
