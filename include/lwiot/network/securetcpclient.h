/*
 * TLS TCP client wrapper.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file securetcpclient.h

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/log.h>
#include <lwiot/types.h>

#include <lwiot/network/tcpclient.h>
#include <lwiot/network/stdnet.h>

namespace lwiot
{
	/**
	 * @brief TLS TCP client wrapper.
	 * @ingroup net
	 */
	class SecureTcpClient : public TcpClient {
	public:
		explicit SecureTcpClient(); //!< Construct a secure TCP client.
		/**
		 * @brief Construct a secure TCP client.
		 * @param addr Remote IP address.
		 * @param port Remote port.
		 * @param host Remote hostname.
		 */
		explicit SecureTcpClient(const IPAddress& addr, uint16_t port, const String& host);

		/**
		 * @brief Copy construct a secure TCP client.
		 * @param other Client to copy.
		 */
		explicit SecureTcpClient(const SecureTcpClient& other);

		/**
		 * @brief Move construct a secure TCP client.
		 * @param other Client to copy.
		 */
		explicit SecureTcpClient(SecureTcpClient&& other) noexcept ;

		~SecureTcpClient() override; //!< Destruct a tcp client.

		SecureTcpClient& operator=(const SecureTcpClient& client); //!< Copy assignment operator.
		SecureTcpClient& operator=(SecureTcpClient&& client) noexcept ; //!< Copy assignment operator.

		bool operator ==(const SecureTcpClient& other); //!< Equality operator.
		bool operator !=(const SecureTcpClient& other); //!< Equality operator.

		/**
		 * @brief Connect to the remote end.
		 * @param addr Remote address.
		 * @param port Remote port.
		 * @return Success indicator.
		 */
		bool connect(const IPAddress& addr, uint16_t port) override;

		/**
		 * @brief Connect to the remote end.
		 * @param host Remote host.
		 * @param port Remote port.
		 * @return Success indicator.
		 */
		bool connect(const String& host, uint16_t port) override;

		/**
		 * @brief Connect to the remote end.
		 * @return A success indicator.
		 */
		bool connect();
		void close() override; //!< Close the socket.

		/**
		 * @brief Set the remote host.
		 * @param host Remote host.
		 */
		void setServerName(const String& host);

		/**
		 * @brief Set the remote certificate.
		 * @param cert The remote certificate.
		 */
		void setServerCertificate(const String& cert);

		explicit operator bool() const override;
		bool connected() const override; //!< Connection check.

		using TcpClient::read;
		using TcpClient::write;

		/**
		 * @brief Read data from the remote.
		 * @param output Output buffer.
		 * @param length Buffer length.
		 * @return Data read.
		 */
		ssize_t read(void *output, const size_t &length) override;

		/**
		 * @brief Write data to the remote end.
		 * @param bytes Buffer.
		 * @param length Buffer length.
		 * @return Data written.
		 */
		ssize_t write(const void *bytes, const size_t& length) override;
		size_t available() const override; //!< Check for available data.

	private:
		secure_socket_t* _socket;
		String _host;
		String _cert;
	};
}
