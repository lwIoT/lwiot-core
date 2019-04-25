/*
 * TLS TCP client wrapper.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/log.h>
#include <lwiot/types.h>

#include <lwiot/network/tcpclient.h>
#include <lwiot/network/stdnet.h>

namespace lwiot
{
	class SecureTcpClient : public TcpClient {
	public:
		explicit SecureTcpClient();
		explicit SecureTcpClient(const IPAddress& addr, uint16_t port, const String& host);
		explicit SecureTcpClient(const SecureTcpClient& other);
		explicit SecureTcpClient(SecureTcpClient&& other);

		virtual ~SecureTcpClient();

		SecureTcpClient& operator=(const SecureTcpClient& client);
		SecureTcpClient& operator=(SecureTcpClient&& client);

		bool operator ==(const SecureTcpClient& other);
		bool operator !=(const SecureTcpClient& other);

		bool connect(const IPAddress& addr, uint16_t port) override;
		bool connect(const String& host, uint16_t port) override;
		bool connect();
		void close() override;

		void setServerName(const String& host);
		void setServerCertificate(const String& cert);

		explicit operator bool() const override;
		bool connected() const override;

		using TcpClient::read;
		using TcpClient::write;

		ssize_t read(void *output, const size_t &length) override;
		ssize_t write(const void *bytes, const size_t& length) override;
		size_t available() const override;

	private:
		secure_socket_t* _socket;
		String _host;
		String _cert;
	};
}
