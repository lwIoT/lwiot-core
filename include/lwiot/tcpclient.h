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
#include <lwiot/string.h>
#include <lwiot/stream.h>
#include <lwiot/ipaddress.h>

#ifdef WIN32
#else
#ifdef HAVE_LWIP
extern "C" {
#include <lwip/sys.h>
#include <lwip/sockets.h>
};
#else
#include <netinet/in.h>
#endif
#endif

namespace lwiot
{
	class TcpClient : public Stream {
	public:
		explicit TcpClient();
		explicit TcpClient(const IPAddress& addr, uint16_t port);
		explicit TcpClient(const String& host, uint16_t port);
		virtual ~TcpClient();

		TcpClient& operator =(const TcpClient& client);

		bool operator ==(const TcpClient& other);
		bool operator !=(const TcpClient& other);

		operator bool() const;
		bool connected() const;

		size_t available() const override;

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

		uint8_t read() override;
		ssize_t read(uint8_t *output, const size_t &length) override;

		void write(uint8_t byte) override;
		void write(const uint8_t *bytes, const size_t &length) override;

		bool connectTo(const IPAddress& addr, uint16_t port);
		bool connectTo(const String& host, uint16_t port);
		void close();

		const IPAddress& remote() const;
		uint16_t port() const;

	private:
		IPAddress _remote_addr;
		uint16_t _remote_port;
		struct sockaddr_in _them;

		int fd;
	};
}
