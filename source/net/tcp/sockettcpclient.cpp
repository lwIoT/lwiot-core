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
#include <lwiot/stl/string.h>
#include <lwiot/error.h>

#include <lwiot/network/stdnet.h>
#include <lwiot/network/sockettcpclient.h>

namespace lwiot
{
	SocketTcpClient::SocketTcpClient() : TcpClient(), _socket(nullptr)
	{
	}

	SocketTcpClient::SocketTcpClient(const lwiot::IPAddress &addr, uint16_t port) : TcpClient(addr, port), _socket(nullptr)
	{
		this->connect();
	}

	SocketTcpClient::SocketTcpClient(const lwiot::String &host, uint16_t port) : TcpClient(0, port), _socket(nullptr)
	{
		remote_addr_t remote;

		dns_resolve_host(host.c_str(), &remote);
		this->_remote_addr = IPAddress(remote);

		this->connect();
	}

	SocketTcpClient::SocketTcpClient(const SocketTcpClient &other) : TcpClient(other._remote_addr, other._remote_port), _socket(nullptr)
	{
		this->connect();
	}

	SocketTcpClient::SocketTcpClient(lwiot::SocketTcpClient &&other) : TcpClient(other._remote_addr, other._remote_port), _socket(nullptr)
	{
		this->_socket = other._socket;
		other._socket = nullptr;
	}

	SocketTcpClient::SocketTcpClient(socket_t *raw) : TcpClient()
	{
		this->_socket = raw;
	}

	SocketTcpClient::~SocketTcpClient()
	{
		if(this->_socket != nullptr) {
			this->close();
		}
	}

	SocketTcpClient& SocketTcpClient::operator=(const lwiot::SocketTcpClient &client)
	{
		if(this->_socket == client._socket)
			return *this;

		if(this->connected())
			this->close();

		this->connect(client._remote_addr, client._remote_port);
		return *this;
	}

	SocketTcpClient& SocketTcpClient::operator=(SocketTcpClient &&client)
	{
		if(this->_socket == client._socket)
			return *this;

		if(this->connected())
			this->close();

		this->_remote_addr = client._remote_addr;
		this->_remote_port = client.port();
		this->_socket = client._socket;

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
		this->_remote_addr = addr;
		this->_remote_port = port;

		return this->connect();
	}

	bool SocketTcpClient::connect()
	{
		remote_addr_t remote;

		this->_remote_addr.toRemoteAddress(remote);
		remote.port = this->_remote_port;

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

	ssize_t SocketTcpClient::write(const void *bytes, const size_t &length)
	{
		return tcp_socket_send(this->_socket, bytes, length);
	}

	ssize_t SocketTcpClient::read(void *output, const size_t &length)
	{
		return tcp_socket_read(this->_socket, output, length);
	}

	void SocketTcpClient::setTimeout(time_t seconds)
	{
		TcpClient::setTimeout(seconds);
		socket_set_timeout(this->_socket, seconds);
	}
}
