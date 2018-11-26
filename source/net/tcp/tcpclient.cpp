/*
 * TCP client wrapper.
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
#include <lwiot/tcpclient.h>
#include <lwiot/error.h>

#if !defined(HAVE_LWIP) && !defined(WIN32)

#include <errno.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

extern "C" {
#include "lwiot/dns.h"
}

namespace lwiot
{
	TcpClient::TcpClient() : _remote_addr((uint32_t)0), _remote_port(0), fd(-1)
	{
	}

	TcpClient::TcpClient(const lwiot::IPAddress &addr, uint16_t port) : _remote_addr(addr), _remote_port(port), fd(-1)
	{
		this->connectTo(addr, port);
	}

	TcpClient::TcpClient(const lwiot::String &host, uint16_t port) :_remote_addr((uint32_t)0), _remote_port(port), fd(-1)
	{
		this->connectTo(host, port);
	}

	TcpClient::~TcpClient()
	{
	}

	TcpClient TcpClient::fromDescriptor(int clientfd)
	{
		TcpClient c;

		c._remote_port = 0;
		c._remote_addr = 0U;
		c.fd = clientfd;
		return c;
	}

	TcpClient& TcpClient::operator=(const lwiot::TcpClient &client)
	{
		if(this->fd == client.fd)
			return *this;

		if(this->connected())
			this->close();

		this->fd = client.fd;
		this->_remote_port = client._remote_port;
		this->_remote_addr = client._remote_addr;
		return *this;
	}

	bool TcpClient::connected() const
	{
		return this->fd >= 0;
	}

	TcpClient::operator bool() const
	{
		return this->connected();
	}

	bool TcpClient::operator==(const lwiot::TcpClient &other)
	{
		return this->fd == other.fd;
	}

	bool TcpClient::operator!=(const lwiot::TcpClient &other)
	{
		return this->fd != other.fd;
	}

	bool TcpClient::connectTo(const lwiot::IPAddress &addr, uint16_t port)
	{
		this->_them.sin_family = AF_INET;
		this->_them.sin_port = htons(port);
		this->_them.sin_addr.s_addr = addr;

		this->fd = socket(AF_INET, SOCK_STREAM, 0);
		if(this->fd < 0) {
			return false;
		}

		if(::connect(this->fd, (struct sockaddr *) &this->_them, sizeof(struct sockaddr)) < 0) {
			this->close();
			return false;
		}

		return true;
	}

	bool TcpClient::connectTo(const lwiot::String &host, uint16_t port)
	{
		const char *hname;
		uint32_t addr;

		hname = host.c_str();
		if(hostname_to_ip(hname, &addr) != -EOK)
			return false;

		return this->connectTo(IPAddress(addr), port);
	}

	void TcpClient::close()
	{
		if(this->fd < 0)
			return;

#ifdef ESP8266
		lwip_close(this->fd);
#else
		::close(this->fd);
#endif
		this->fd = -1;
	}

	const IPAddress& TcpClient::remote() const
	{
		return this->_remote_addr;
	}

	uint16_t TcpClient::port() const
	{
		return this->_remote_port;
	}

	void TcpClient::write(const void *bytes, const size_t &length)
	{
		this->write(reinterpret_cast<const uint8_t *>(bytes), length);
	}

	size_t TcpClient::available() const
	{
		int count;

#if defined(WIN32) || defined(HAVE_LWIP)
		ioctlsocket(this->fd, FIONREAD, &count);
#else
		ioctl(this->fd, FIONREAD, &count);
#endif

		return static_cast<size_t>(count);
	}

	uint8_t TcpClient::read()
	{
		uint8_t tmp;

		auto rc = this->read(&tmp, sizeof(tmp));
		if(rc < 0)
			return 0;

		return tmp;
	}

	ssize_t TcpClient::read(void *output, const size_t &length)
	{
		return this->read(reinterpret_cast<uint8_t *>(output), length);
	}

	void TcpClient::write(uint8_t byte)
	{
		this->write(&byte, sizeof(byte));
	}

	void TcpClient::write(const uint8_t *bytes, const size_t &length)
	{
#ifdef HAVE_LWIP
		lwip_send(this->fd, bytes, length, 0);
#else
		send(this->fd, bytes, length, 0);
#endif
	}

	ssize_t TcpClient::read(uint8_t *output, const size_t &length)
	{
		ssize_t retval;

#ifdef HAVE_LWIP
		retval = lwip_recv(this->fd, output, length, 0);
#else
		retval = recv(this->fd, output, length, 0);
#endif

		return retval;
	}

	Stream& TcpClient::operator<<(char x)
	{
		this->write((uint8_t)x);
		return *this;
	}

	Stream& TcpClient::operator<<(short x)
	{
		this->write((uint8_t*)&x, sizeof(x));
		return *this;
	}

	Stream& TcpClient::operator<<(int x)
	{
		this->write((uint8_t*)&x, sizeof(x));
		return *this;
	}

	Stream& TcpClient::operator<<(const long & x)
	{
		this->write((uint8_t*)&x, sizeof(x));
		return *this;
	}

	Stream& TcpClient::operator<<(const long long& x)
	{
		this->write((uint8_t*)&x, sizeof(x));
		return *this;
	}

	Stream& TcpClient::operator<<(unsigned char x)
	{
		this->write((uint8_t)x);
		return *this;
	}

	Stream& TcpClient::operator<<(unsigned short x)
	{
		this->write((uint8_t*)&x, sizeof(x));
		return *this;
	}

	Stream& TcpClient::operator<<(unsigned int x)
	{
		this->write((uint8_t*)&x, sizeof(x));
		return *this;
	}

	Stream& TcpClient::operator<<(unsigned const long & x)
	{
		this->write((uint8_t*)&x, sizeof(x));
		return *this;
	}

	Stream& TcpClient::operator<<(const unsigned long long& x)
	{
		this->write((uint8_t*)&x, sizeof(x));
		return *this;
	}

	Stream& TcpClient::operator<<(const float &x)
	{
		this->write((uint8_t*)&x, sizeof(x));
		return *this;
	}

	Stream& TcpClient::operator<<(const double &x)
	{
		this->write((uint8_t*)&x, sizeof(x));
		return *this;
	}

	Stream& TcpClient::operator<<(const lwiot::String &str)
	{
		*this << str.c_str();
		return *this;
	}

	Stream& TcpClient::operator<<(const char *cstr)
	{
		this->write(reinterpret_cast<const uint8_t *>(cstr), strlen(cstr));
		return *this;
	}
}
