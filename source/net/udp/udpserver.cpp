/*
 * UDP server wrapper class.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/log.h>
#include <lwiot/string.h>
#include <lwiot/error.h>

#include <lwiot/network/stdnet.h>
#include <lwiot/network/udpclient.h>
#include <lwiot/network/udpserver.h>
#include <lwiot/stl/move.h>

namespace lwiot
{
	UdpServer::UdpServer() : _bind_addr(0U), _bind_port(0)
	{
	}

	UdpServer::UdpServer(const IPAddress& addr, uint16_t port) :
		_bind_addr(addr), _bind_port(to_netorders(port))
	{
	}

	bool UdpServer::bind(const lwiot::IPAddress &addr, uint16_t port)
	{
		this->_bind_addr = lwiot::stl::move(addr);
		this->_bind_port = to_netorders(port);

		return true;
	}

	const IPAddress& UdpServer::address() const
	{
		return this->_bind_addr;
	}

	uint16_t UdpServer::port() const
	{
		return this->_bind_port;
	}
}
