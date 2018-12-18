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
	UdpServer::UdpServer() : _bind_addr(0U), _bind_port(0), _bindaddr(BIND_ADDR_ANY)
	{
	}

	UdpServer::UdpServer(bind_addr_t addr, uint16_t port) :
		_bind_addr(IPAddress::fromBindAddress(addr)), _bind_port(to_netorders(port)), _bindaddr(addr)
	{
	}

	bool UdpServer::bind(bind_addr_t addr, uint16_t port)
	{
		this->_bind_addr = lwiot::stl::move(IPAddress::fromBindAddress(addr));
		this->_bind_port = to_netorders(port);
		this->_bindaddr = addr;

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

	bind_addr_t UdpServer::bindaddr() const
	{
		return this->_bindaddr;
	}
}
