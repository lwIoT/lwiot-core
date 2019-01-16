/*
 * IP address object implementation.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/stl/string.h>

#include <lwiot/network/ipaddress.h>
#include <lwiot/network/stdnet.h>

namespace lwiot
{
	IPAddress::IPAddress() : _version(4)
	{
		this->_address.dword[0] = 0;
	}

	IPAddress::IPAddress(uint8_t first, uint8_t second, uint8_t third, uint8_t forth) : _version(4)
	{
		this->_address.bytes[0] = first;
		this->_address.bytes[1] = second;
		this->_address.bytes[2] = third;
		this->_address.bytes[3] = forth;
	}

	IPAddress::IPAddress(uint32_t addr) : _version(4)
	{
		this->_address.dword[0] = to_netorderl(addr);
	}

	IPAddress::IPAddress(const uint8_t *addr) : IPAddress(addr[0], addr[1], addr[2], addr[3])
	{ }

	IPAddress::IPAddress(const remote_addr_t& remote)
	{
		if(remote.version == 6) {
			memcpy(this->_address.bytes, remote.addr.ip6_addr.ip, sizeof(remote.addr.ip6_addr.ip));
			this->_version = 6;
		} else {
			this->_address.dword[0] = remote.addr.ip4_addr.ip;
			this->_version = 4;
		}

	}

	const uint8_t *IPAddress::raw() const
	{
		return (const uint8_t*)this->_address.bytes;
	}

	String IPAddress::toString() const
	{
		char tmp[16];

		memset((void*)tmp, 0, sizeof(tmp));
		sprintf(tmp, "%u.%u.%u.%u", this->_address.bytes[0], this->_address.bytes[1],
				this->_address.bytes[2], this->_address.bytes[3]);
		return String(tmp);
	}

	IPAddress IPAddress::fromString(const char *str)
	{
		IPAddress retval;

		int acc = 0;
		uint8_t dots = 0;

		while (*str) {
			auto c = *str++;
			if (c >= '0' && c <= '9') {
				acc = acc * 10 + (c - '0');

				if (acc > 255)
					return retval;

			} else if (c == '.') {
				if (dots == 3)
					return retval;

				retval._address.bytes[dots++] = (uint8_t)acc;
				acc = 0;
			} else {
				return retval;
			}
		}

		if (dots != 3)
			return retval;

		retval._address.bytes[3] = (uint8_t)acc;
		retval._version = 4;
		return retval;
	}

	IPAddress IPAddress::fromString(const String& str)
	{
		return IPAddress::fromString(str.c_str());
	}

	IPAddress& IPAddress::operator=(const uint8_t *address)
	{
		memcpy(this->_address.bytes, address, sizeof(this->_address.bytes));
		return *this;
	}

	IPAddress& IPAddress::operator=(uint32_t address)
	{
		this->_address.dword[0] = address;
		this->_version = 4;
		return *this;
	}

	IPAddress& IPAddress::operator=(const IPAddress& addr)
	{
		memcpy(this->_address.bytes, addr._address.bytes, sizeof(_address.bytes));
		this->_version = addr._version;
		return *this;
	}

	bool IPAddress::operator==(const uint8_t* addr) const
	{
		return memcmp(addr, this->_address.bytes, sizeof(this->_address.bytes)) == 0;
	}

	bool IPAddress::operator==(const uint32_t& other) const
	{
		return this->_address.dword[0] == other;
	}

	bool IPAddress::operator==(const IPAddress& addr) const
	{
		return memcmp(addr.raw(), this->_address.bytes, sizeof(this->_address.bytes)) == 0;
	}

	IPAddress::operator uint32_t() const
	{
		return this->_address.dword[0];
	}

	uint8_t IPAddress::operator [](int idx) const
	{
		if((unsigned)idx >= sizeof(this->_address.bytes))
			return 0;

		return this->_address.bytes[idx];
	}

	uint8_t& IPAddress::operator [](int idx)
	{
		if((unsigned)idx >= sizeof(this->_address.bytes))
			return this->_address.bytes[0];

		return this->_address.bytes[idx];
	}

	void IPAddress::toRemoteAddress(remote_addr_t& remote) const
	{
		if(this->_version == 4) {
			remote.addr.ip4_addr.ip = this->_address.dword[0];
		} else {
			memcpy(remote.addr.ip6_addr.ip, this->_address.bytes, sizeof(remote.addr.ip6_addr.ip));
		}

		remote.version = this->version();
	}

	IPAddress IPAddress::fromBindAddress(lwiot::BindAddress addr)
	{
		IPAddress ip;

		switch(addr) {
		case BIND_ADDR_ANY:
			ip._address.dword[0] = 0U;
			break;

		case BIND_ADDR_LB:
			ip._address.dword[0] = 0U;
			ip._address.bytes[0] = 127;
			ip._address.bytes[3] = 1;
			break;

		case BIND6_ADDR_ANY:
		memset(ip._address.bytes, 0, sizeof(ip._address.bytes));
		/*case BIND6_ADDR_LB:
#ifdef HAVE_BIG_ENDIAN
			uint32_t raw = 1;
#else
			uint32_t raw = 16777216;
#endif
			ip._address.dword[0] = raw;
			ip._address.dword[1] = 0;
			ip._address.dword[2] = 0;
			ip._address.dword[3] = 0;
			break;*/
		}

		return ip;
	}
}
