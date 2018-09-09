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
#include <lwiot/ipaddress.h>
#include <lwiot/tcpserver.h>

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
	TcpServer::TcpServer() : _fd(-1)
	{
		this->_fd = socket(AF_INET, SOCK_STREAM, 0);
	}

	TcpServer::TcpServer(const lwiot::IPAddress &addr, uint16_t port) : _bind_addr(addr), _bind_port(port), _fd(-1)
	{
		this->_fd = socket(AF_INET, SOCK_STREAM, 0);
	}

	TcpServer::~TcpServer()
	{
		if(this->_fd < 0)
			this->close();
	}

	TcpServer& TcpServer::operator=(const lwiot::TcpServer &server)
	{
		if(server._fd == this->_fd)
			return *this;

		this->close();
		this->_bind_addr = server._bind_addr;
		this->_bind_port = server._bind_port;
		this->bind();

		return *this;
	}

	bool TcpServer::operator==(const lwiot::TcpServer &other)
	{
		return this->_fd == other._fd;
	}

	bool TcpServer::operator!=(const lwiot::TcpServer &other)
	{
		return this->_fd != other._fd;
	}

	void TcpServer::close()
	{
		if(this->_fd < 0)
			return;

#ifdef ESP8266
		lwip_close(this->fd);
#else
		::close(this->_fd);
#endif

		this->_fd = -1;
	}

	bool TcpServer::bind(const lwiot::IPAddress &addr, uint16_t port)
	{
		this->_bind_port = port;
		this->_bind_addr = addr;

		if(this->_fd < 0)
			this->_fd = socket(AF_INET, SOCK_STREAM, 0);

		return this->bind();
	}

	bool TcpServer::bind() const
	{
		struct sockaddr_in server;
		int optvalue = 1;

		if(this->_fd < 0) {
			print_dbg("Invalid socket descriptor, unable to bind!\n");
			return false;
		}

		memset(server.sin_zero, 0, sizeof(server.sin_zero));
		server.sin_port = htons(this->_bind_port);
		server.sin_addr.s_addr = this->_bind_addr;
		server.sin_family = AF_INET;
#ifdef HAVE_LWIP
		server.sin_len = sizeof(server);
#endif

		setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &optvalue, sizeof optvalue);
#ifdef HAVE_LWIP
		auto err = lwip_bind(this->_fd, (struct sockaddr*)&server, sizeof server);
#else
		auto err = ::bind(this->_fd, (struct sockaddr*)&server, sizeof server);
#endif

		if(err < 0) {
			print_dbg("Unable to bind [fd: %i] - err: %i\n", this->_fd, err);
			return false;
		}

#ifdef HAVE_LWIP
		err = lwip_listen(this->_fd, TcpServer::BacklogSize);

		if(err < 0) {
			print_dbg("Unable to listen [fd: %i] - err: %i\n", this->_fd, err);
			return false;
		}
#else
		err = ::listen(this->_fd, TcpServer::BacklogSize);
#endif

		print_dbg("Server binded [fd: %i] [err: %i]\n", this->_fd, err);
		return err >= 0;

	}

	TcpClient TcpServer::accept() const
	{
		struct sockaddr_in client;
		int fd;
		socklen_t len;

		if(this->_fd < 0)
			return TcpClient();

		len = sizeof(client);
#ifdef HAVE_LWIP
		fd = lwip_accept(this->_fd, (struct sockaddr*) &client, &len);
#else
		fd = ::accept(this->_fd, (struct sockaddr*) &client, &len);
#endif

		return TcpClient::fromDescriptor(fd);
	}
}
