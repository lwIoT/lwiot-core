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
#include <lwiot/network/socketudpserver.h>
#include <lwiot/network/socketudpclient.h>

namespace lwiot
{
	SocketUdpServer::SocketUdpServer() : UdpServer()
	{
		this->_socket = server_socket_create(SOCKET_DGRAM, this->address().isIPv6());
	}

	SocketUdpServer::SocketUdpServer(bind_addr_t addr, uint16_t port ) : UdpServer(addr, port)
	{
		this->_socket = server_socket_create(SOCKET_DGRAM, this->address().isIPv6());
	}

	SocketUdpServer::~SocketUdpServer()
	{
		this->close();
	}

	void SocketUdpServer::close()
	{
		assert(this->_socket != nullptr);
		socket_close(this->_socket);
	}

	bool SocketUdpServer::bind(bind_addr_t addr, uint16_t port)
	{
		UdpServer::bind(addr, port);
		return this->bind();
	}

	bool SocketUdpServer::bind()
	{
		if(!this->_socket)
			this->_socket = server_socket_create(SOCKET_DGRAM, this->address().isIPv6());

		assert(this->_socket != nullptr);
		return server_socket_bind(this->_socket, this->bindaddr(), this->port());
	}

	UniquePointer<UdpClient> SocketUdpServer::recv(void *buffer, size_t length)
	{
		remote_addr_t remote;
		UniquePointer<UdpClient> client;

		remote.version = this->address().version();
		if(udp_recv_from(this->_socket, buffer, length, &remote) < 0) {
			return client;
		}

		/* Convert to host order, as UdpClient expects the port number to be supplied in host order */
		remote.port = to_hostorders(remote.port);
		auto raw = new SocketUdpClient(IPAddress(remote), remote.port, this->_socket);
		client.reset(raw);

		return client;
	}
}
