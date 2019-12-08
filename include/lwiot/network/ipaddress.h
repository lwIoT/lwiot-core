/*
 * IP address object definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file ipaddress.h

#pragma once

#include <lwiot/types.h>
#include <lwiot/stl/string.h>

#include <lwiot/network/stdnet.h>

namespace lwiot
{
	/**
	 * @brief Bind address.
	 * @ingroup net
	 */
	using BindAddress = bind_addr_t;

	/**
	 * @brief IP address wrapper.
	 * @ingroup net
	 */
	class IPAddress {
	public:
		explicit IPAddress(); //!< Construct an IP address.

		/**
		 * @brief Construct an IP address.
		 * @param first First ocet.
		 * @param second Second ocet.
		 * @param third Third ocet.
		 * @param forth Forth ocet.
		 */
		explicit IPAddress(uint8_t first, uint8_t second, uint8_t third, uint8_t forth);

		/**
		 * @brief Construct an IP address.
		 * @param address 32-bit address buffer.
		 */
		explicit IPAddress(const uint8_t *address);

		/**
		 * @brief Construct an IP address.
		 * @param address 32-bit address.
		 */
		explicit IPAddress(uint32_t address);

		/**
		 * @brief Construct an IP address.
		 * @param remote Remote address.
		 */
		explicit IPAddress(const remote_addr_t& remote);
		virtual ~IPAddress() = default;

		virtual String toString() const; //!< Conver the IP address to a string.

		/**
		 * @brief Convert a string to an IP address object.
		 * @param str String to convert.
		 * @return An IP address object.
		 */
		static IPAddress fromString(const String& str);

		/**
		 * @brief Convert a string to an IP address object.
		 * @param str String to convert.
		 * @return An IP address object.
		 */
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

		void toRemoteAddress(remote_addr_t& remote) const; //!< Convert to a remote address.

		int version() const { return this->_version; } //!< Get the IP version.
		bool isIPv6() const { return this->_version == 6; } //!< Check IPv6.

		static IPAddress fromBindAddress(BindAddress addr); //!< Convert to an IP address object.

	private:
		union {
			uint8_t bytes[16];
			uint32_t dword[4];
		} _address;

		int _version;

		const uint8_t *raw() const;
	};
}
