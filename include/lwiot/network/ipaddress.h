/*
 * IP address object definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/types.h>
#include <lwiot/stl/string.h>

#include <lwiot/network/stdnet.h>

namespace lwiot
{
	using BindAddress = bind_addr_t;

	class IPAddress {
	public:
		explicit IPAddress();
		explicit IPAddress(uint8_t first, uint8_t second, uint8_t third, uint8_t forth);
		explicit IPAddress(const uint8_t *address);
		explicit IPAddress(uint32_t address);
		explicit IPAddress(const remote_addr_t& remote);
		virtual ~IPAddress() = default;

		virtual String toString() const;
		static IPAddress fromString(const String& str);
		static IPAddress fromString(const char *str);

		/* Operators */
		operator uint32_t () const;
		bool operator ==(const IPAddress& other) const;
		bool operator ==(const uint32_t& other) const;
		bool operator ==(const uint8_t* other) const;
		uint8_t operator [](int idx) const;
		uint8_t& operator [](int idx);

		IPAddress& operator =(const uint8_t *address);
		IPAddress& operator =(uint32_t addr);
		IPAddress& operator =(const IPAddress& addr);

		void toRemoteAddress(remote_addr_t& remote) const;

		int version() const { return this->_version; }
		bool isIPv6() const { return this->_version == 6; }

		static IPAddress fromBindAddress(BindAddress addr);

	private:
		union {
			uint8_t bytes[16];
			uint32_t dword[4];
		} _address;
		int _version;

		const uint8_t *raw() const;
	};
}
