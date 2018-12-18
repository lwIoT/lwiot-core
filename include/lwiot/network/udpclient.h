/*
 * UDP client wrapper class.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/network/ipaddress.h>

namespace lwiot
{
	class UdpClient {
	public:
		explicit UdpClient();
		explicit UdpClient(const IPAddress& addr, uint16_t port);
		explicit UdpClient(const String& host, uint16_t port);
		virtual ~UdpClient() = default;

		virtual void close() = 0;
		virtual ssize_t read(void *buffer, size_t length) = 0;
		virtual ssize_t write(const void *buffer, size_t length) = 0;

		const IPAddress& address() const;
		uint16_t port() const;

	private:
		IPAddress _remote;
		uint16_t _port;
	};
}
