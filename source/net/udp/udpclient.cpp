/*
 * UDP client implementation.
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
#include <lwiot/network/ipaddress.h>

#include <lwiot/stl/move.h>

lwiot::UdpClient::UdpClient() : _remote(IPAddress::fromBindAddress(BIND_ADDR_ANY)), _port(0)
{
}

lwiot::UdpClient::UdpClient(const lwiot::IPAddress &addr, uint16_t port) : _remote(addr), _port(to_netorders(port))
{
}

lwiot::UdpClient::UdpClient(const lwiot::String &host, uint16_t port) : _remote((uint32_t)0), _port(to_netorders(port))
{
	remote_addr_t remote;

	if(dns_resolve_host(host.c_str(), &remote) == -EOK) {
		this->_remote = lwiot::stl::move(IPAddress(remote));
	}
}

const lwiot::IPAddress& lwiot::UdpClient::address() const
{
	return this->_remote;
}

uint16_t lwiot::UdpClient::port() const
{
	return this->_port;
}
