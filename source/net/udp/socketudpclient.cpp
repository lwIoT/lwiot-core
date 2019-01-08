/*
 * UDP server wrapper class.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/log.h>
#include <lwiot/string.h>
#include <lwiot/error.h>

#include <lwiot/network/stdnet.h>
#include <lwiot/network/udpclient.h>
#include <lwiot/network/socketudpclient.h>

namespace lwiot
{
	SocketUdpClient::SocketUdpClient()
	{
	}

	SocketUdpClient::SocketUdpClient(const IPAddress &addr, uint16_t port, socket_t* srv) :
		UdpClient(addr, port), _socket(srv)
	{
		if(srv == nullptr)
			this->init();
		else
			this->_noclose = true;
	}

	SocketUdpClient::SocketUdpClient(const lwiot::String &host, uint16_t port) : UdpClient(host, port)
	{
		this->init();
	}

	void SocketUdpClient::init()
	{
		remote_addr_t remote;

		this->address().toRemoteAddress(remote);
		this->_socket = udp_socket_create(&remote);
		this->_noclose = false;
	}

	SocketUdpClient::~SocketUdpClient()
	{
		this->close();
	}

	void SocketUdpClient::close()
	{
		if(!this->_noclose) {
			socket_close(this->_socket);
		}
	}

	ssize_t SocketUdpClient::write(const void *buffer, const size_t& length)
	{
		remote_addr_t remote;

		assert(this->_socket != nullptr);
		this->address().toRemoteAddress(remote);
		remote.version = this->address().version();
		remote.port = this->port();
		return udp_send_to(this->_socket, buffer, length, &remote);
	}

	ssize_t SocketUdpClient::read(void *buffer, const size_t& length)
	{
		remote_addr_t remote;

		assert(this->_socket != nullptr);
		remote.version = this->address().version();
		return udp_recv_from(this->_socket, buffer, length, &remote);
	}

	size_t SocketUdpClient::available() const
	{
		return udp_socket_available(this->_socket);
	}
}
