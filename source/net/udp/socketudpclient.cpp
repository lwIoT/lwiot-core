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
#include <lwiot/stl/string.h>
#include <lwiot/error.h>

#include <lwiot/network/stdnet.h>
#include <lwiot/network/udpclient.h>
#include <lwiot/network/socketudpclient.h>

namespace lwiot
{
	SocketUdpClient::SocketUdpClient() : UdpClient(), _socket(nullptr), _noclose(false)
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

	void SocketUdpClient::begin()
	{
		this->resolve();
		this->close();
		this->init();
	}

	void SocketUdpClient::begin(const lwiot::String &host, uint16_t port)
	{
		this->_host = host;
		this->_port = to_netorders(port);
		this->begin();
	}

	void SocketUdpClient::begin(const lwiot::IPAddress &addr, uint16_t port)
	{
		this->_host = "";
		this->_remote = addr;
		this->_port = to_netorders(port);
		this->begin();
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
		if(!this->_noclose && this->_socket != nullptr) {
			socket_close(this->_socket);
			this->_socket = nullptr;
		}
	}

	void SocketUdpClient::setTimeout(time_t seconds)
	{
		UdpClient::setTimeout(seconds);
		socket_set_timeout(this->_socket, seconds);
	}

	ssize_t SocketUdpClient::write(const void *buffer, const size_t& length)
	{
		remote_addr_t remote;

		if(this->_socket == nullptr) {
			this->resolve();
			this->init();
		}

		if(this->_socket == nullptr)
			return -EINVALID;

		this->address().toRemoteAddress(remote);
		remote.version = this->address().version();
		remote.port = this->port();
		return udp_send_to(this->_socket, buffer, length, &remote);
	}

	ssize_t SocketUdpClient::read(void *buffer, const size_t& length)
	{
		remote_addr_t remote;

		if(this->_socket == nullptr) {
			this->resolve();
			this->init();
		}

		if(this->_socket == nullptr)
			return -EINVALID;

		remote.version = this->address().version();
		return udp_recv_from(this->_socket, buffer, length, &remote);
	}

	size_t SocketUdpClient::available() const
	{
		if(this->_socket == nullptr)
			return -EINVALID;

		return udp_socket_available(this->_socket);
	}
}
