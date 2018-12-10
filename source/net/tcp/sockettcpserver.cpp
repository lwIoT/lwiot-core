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
#include <lwiot/network/sockettcpclient.h>
#include <lwiot/network/sockettcpserver.h>

#if !defined(HAVE_LWIP) && !defined(WIN32)
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

extern "C" {
#include "lwiot/dns.h"
}

namespace lwiot
{
	SocketTcpServer::SocketTcpServer() : TcpServer()
	{
		this->_socket = server_socket_create(SOCKET_STREAM, false);
	}

	SocketTcpServer::SocketTcpServer(const lwiot::IPAddress &addr, uint16_t port) : TcpServer(addr, port)
	{
		this->_socket = server_socket_create(SOCKET_STREAM, addr.isIPv6());
	}

	SocketTcpServer::~SocketTcpServer()
	{
		if(this->_socket != nullptr)
			this->close();
	}

	SocketTcpServer& SocketTcpServer::operator=(const lwiot::SocketTcpServer &server)
	{
		if(server._socket == this->_socket)
			return *this;

		this->close();
		this->bind();

		return *this;
	}

	void SocketTcpServer::connect()
	{
		if(this->_socket)
			this->close();

		this->_socket = server_socket_create(SOCKET_STREAM, this->address().isIPv6());
	}

	bool SocketTcpServer::operator==(const lwiot::SocketTcpServer &other)
	{
		return this->_socket == other._socket;
	}

	bool SocketTcpServer::operator!=(const lwiot::SocketTcpServer &other)
	{
		return this->_socket != other._socket;
	}

	void SocketTcpServer::close()
	{
		if(this->_socket == nullptr)
			return;

		socket_close(this->_socket);
	}

	bool SocketTcpServer::bind(const lwiot::IPAddress &addr, uint16_t port)
	{
		this->_bind_port = port;
		this->_bind_addr = addr;

		if(this->_socket == nullptr)
			this->connect();

		return this->bind();
	}

	bool SocketTcpServer::bind() const
	{
		bool rv;

		if(this->_socket == nullptr) {
			print_dbg("Invalid socket descriptor, unable to bind!\n");
			return false;
		}

		if(this->_bind_addr.isIPv6()) {
			rv = server_socket_bind(this->_socket, BIND6_ADDR_ANY, this->_bind_port);
		} else {
			rv = server_socket_bind(this->_socket, BIND_ADDR_ANY, this->_bind_port);
		}

		if(!rv) {
			print_dbg("Unable to bind TCP server socket!\n");
			return false;
		}

		rv = server_socket_listen(this->_socket);

		if(!rv) {
			print_dbg("Unable to accept client sockets!\n");
			return false;
		}

		print_dbg("Server binded!\n");
		return true;
	}

	UniquePointer<TcpClient> SocketTcpServer::accept()
	{
		auto socket = server_socket_accept(this->_socket);
		auto client = new SocketTcpClient(socket);
		UniquePointer<TcpClient> wrapped(client);

		return wrapped;
	}
}
