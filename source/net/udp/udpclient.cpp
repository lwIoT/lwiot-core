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
#include <lwiot/stl/string.h>
#include <lwiot/error.h>

#include <lwiot/network/stdnet.h>
#include <lwiot/network/udpclient.h>
#include <lwiot/network/ipaddress.h>

#include <lwiot/stl/move.h>

namespace lwiot
{
	UdpClient::UdpClient() : _remote(IPAddress::fromBindAddress(BIND_ADDR_ANY)), _port(0)
	{
	}

	UdpClient::UdpClient(const lwiot::IPAddress &addr, uint16_t port) : _remote(addr), _port(to_netorders(port))
	{
	}

	UdpClient::UdpClient(const lwiot::String &host, uint16_t port) : _remote((uint32_t)0), _port(to_netorders(port)), _host(host)
	{
		this->resolve();
	}

	const IPAddress& lwiot::UdpClient::address() const
	{
		return this->_remote;
	}

	uint16_t UdpClient::port() const
	{
		return this->_port;
	}

	void UdpClient::resolve()
	{
		remote_addr_t remote;

		if(this->_host.length() <= 0)
			return;


		remote.version = 4;
		dns_resolve_host(this->_host.c_str(), &remote);
		this->_remote = stl::move(IPAddress(remote));
	}

	uint8_t UdpClient::read()
	{
		uint8_t tmp;

		auto rc = this->read(&tmp, sizeof(tmp));
		if(rc < 0)
			return 0;

		return tmp;
	}

	bool UdpClient::write(uint8_t byte)
	{
		return this->write(&byte, sizeof(byte)) == sizeof(byte);
	}

	Stream& UdpClient::operator<<(char x)
	{
		this->write((uint8_t)x);
		return *this;
	}

	Stream& UdpClient::operator<<(short x)
	{
		this->write((uint8_t*)&x, sizeof(x));
		return *this;
	}

	Stream& UdpClient::operator<<(int x)
	{
		this->write((uint8_t*)&x, sizeof(x));
		return *this;
	}

	Stream& UdpClient::operator<<(const long & x)
	{
		this->write((uint8_t*)&x, sizeof(x));
		return *this;
	}

	Stream& UdpClient::operator<<(const long long& x)
	{
		this->write((uint8_t*)&x, sizeof(x));
		return *this;
	}

	Stream& UdpClient::operator<<(unsigned char x)
	{
		this->write((uint8_t)x);
		return *this;
	}

	Stream& UdpClient::operator<<(unsigned short x)
	{
		this->write((uint8_t*)&x, sizeof(x));
		return *this;
	}

	Stream& UdpClient::operator<<(unsigned int x)
	{
		this->write((uint8_t*)&x, sizeof(x));
		return *this;
	}

	Stream& UdpClient::operator<<(unsigned const long & x)
	{
		this->write((uint8_t*)&x, sizeof(x));
		return *this;
	}

	Stream& UdpClient::operator<<(const unsigned long long& x)
	{
		this->write((uint8_t*)&x, sizeof(x));
		return *this;
	}

	Stream& UdpClient::operator<<(const float &x)
	{
		this->write((uint8_t*)&x, sizeof(x));
		return *this;
	}

	Stream& UdpClient::operator<<(const double &x)
	{
		this->write((uint8_t*)&x, sizeof(x));
		return *this;
	}

	Stream& UdpClient::operator<<(const lwiot::String &str)
	{
		*this << str.c_str();
		return *this;
	}

	Stream& UdpClient::operator<<(const char *cstr)
	{
		this->write(reinterpret_cast<const uint8_t *>(cstr), strlen(cstr));
		return *this;
	}
}
