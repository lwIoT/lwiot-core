/*
 * TCP client wrapper.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>
#include <assert.h>

#include <lwiot/types.h>
#include <lwiot/log.h>
#include <lwiot/string.h>
#include <lwiot/sockettcpclient.h>
#include <lwiot/error.h>
#include <lwiot/stdnet.h>

namespace lwiot
{
	SocketTcpClient::SocketTcpClient() : TcpClient(), _socket(nullptr)
	{
	}

	SocketTcpClient::SocketTcpClient(const lwiot::IPAddress &addr, uint16_t port) : TcpClient(addr, port), _socket(nullptr)
	{
		this->connect(addr, port);
	}

	SocketTcpClient::SocketTcpClient(const lwiot::String &host, uint16_t port) : TcpClient(host, port), _socket(nullptr)
	{
		this->connect(host, port);
	}

	SocketTcpClient::~SocketTcpClient()
	{
		if(this->_socket != nullptr) {
			this->close();
		}
	}

	SocketTcpClient SocketTcpClient::fromDescriptor(socket_t* client_socket)
	{
		SocketTcpClient c;

		c._remote_port = 0;
		c._remote_addr = 0U;
		c._socket = client_socket;
		return c;
	}

	SocketTcpClient& SocketTcpClient::operator=(const lwiot::SocketTcpClient &client)
	{
		if(this->_socket == client._socket)
			return *this;

		if(this->connected())
			this->close();

		this->_socket = client._socket;
		this->_remote_port = client._remote_port;
		this->_remote_addr = client._remote_addr;
		return *this;
	}

	bool SocketTcpClient::connected() const
	{
		return this->_socket != nullptr;
	}

	SocketTcpClient::operator bool() const
	{
		return this->connected();
	}

	bool SocketTcpClient::operator==(const lwiot::SocketTcpClient &other)
	{
		return this->_socket == other._socket;
	}

	bool SocketTcpClient::operator!=(const lwiot::SocketTcpClient &other)
	{
		return this->_socket != other._socket;
	}

	bool SocketTcpClient::connect(const lwiot::IPAddress &addr, uint16_t port)
	{
		remote_addr_t remote;

		this->_remote_addr = addr;
		this->_remote_port = port;

		addr.toRemoteAddress(remote);
		remote.port = port;

		this->_socket = tcp_socket_create(&remote);

		return this->_socket != nullptr;
	}

	bool SocketTcpClient::connect(const lwiot::String &host, uint16_t port)
	{
		const char *hname;
		remote_addr_t remote;

		hname = host.c_str();
		if(dns_resolve_host(hname, &remote) != -EOK)
			return false;

		this->_socket = tcp_socket_create(&remote);
		return this->_socket != nullptr;
	}

	void SocketTcpClient::close()
	{
		if(!this->connected())
			return;

		socket_close(this->_socket);
		this->_socket = nullptr ;
	}

	size_t SocketTcpClient::available() const
	{
		return tcp_socket_available(this->_socket);
	}

	void SocketTcpClient::write(const void *bytes, const size_t &length)
	{
		auto sent = tcp_socket_send(this->_socket, bytes, length);
		assert(sent = length);
	}

	ssize_t SocketTcpClient::read(void *output, const size_t &length)
	{
		return tcp_socket_read(this->_socket, output, length);
	}
}
