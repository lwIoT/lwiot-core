/*
 * UDP client wrapper class.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/types.h>
#include <lwiot/stream.h>

#include <lwiot/network/ipaddress.h>

namespace lwiot
{
	class UdpClient : public Stream {
	public:
		explicit UdpClient();
		explicit UdpClient(const IPAddress& addr, uint16_t port);
		explicit UdpClient(const String& host, uint16_t port);
		virtual ~UdpClient() = default;

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

		virtual void close() = 0;

		using Stream::read;
		uint8_t read() override;

		using Stream::write;
		bool write(uint8_t byte) override;

		const IPAddress& address() const;
		uint16_t port() const;

	private:
		IPAddress _remote;
		uint16_t _port;
	};
}
