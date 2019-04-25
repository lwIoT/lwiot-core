/*
 * TCP client wrapper.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/log.h>
#include <lwiot/stl/string.h>
#include <lwiot/stream.h>
#include <lwiot/network/ipaddress.h>
#include <lwiot/network/stdnet.h>

namespace lwiot
{
	class TcpClient : public Stream {
	public:
		explicit TcpClient();
		explicit TcpClient(const IPAddress& addr, uint16_t port);
		explicit TcpClient(const String& host, uint16_t port);
		~TcpClient() override = default;

		virtual TcpClient& operator =(const TcpClient& client);
		virtual TcpClient& operator =(TcpClient&& client);

		virtual bool operator ==(const TcpClient& other);
		virtual bool operator !=(const TcpClient& other);

		virtual explicit operator bool() const = 0;
		virtual bool connected() const = 0;

		using Stream::available;

		Stream &operator<<(char x) override;
		Stream &operator<<(short x) override;
		Stream &operator<<(int x) override;
		Stream &operator<<(const long &x) override;
		Stream &operator<<(const long long &x) override;
		Stream &operator<<(unsigned char x) override;
		Stream &operator<<(unsigned short x) override;
		Stream &operator<<(unsigned int x) override;
		Stream &operator<<(const unsigned long &x) override;
		Stream &operator<<(const unsigned long long &x) override;
		Stream &operator<<(const double &x) override;
		Stream &operator<<(const float &x) override;
		Stream &operator<<(const String &str) override;
		Stream &operator<<(const char *cstr) override;

		using Stream::read;
		uint8_t read() override;

		using Stream::write;
		bool write(uint8_t byte) override;

		virtual bool connect(const IPAddress& addr, uint16_t port) = 0;
		virtual bool connect(const String& host, uint16_t port)    = 0;

		virtual void close() = 0;

		const IPAddress& remote() const;
		uint16_t port() const;

	protected:
		IPAddress _remote_addr;
		uint16_t _remote_port;
	};
}
