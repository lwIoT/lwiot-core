/*
 * TCP client wrapper.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file sockettcpclient.h

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
#include <lwiot/network/tcpclient.h>

namespace lwiot
{
	/**
	 * @brief Socket TCP client wrapper.
	 */
	class SocketTcpClient : public TcpClient {
	public:
		explicit SocketTcpClient(); //!< Construct a socket TCP client.

		/**
		 * @brief Construct a socket wrapper from a raw socket.
		 * @param raw Raw socket.
		 */
		explicit SocketTcpClient(socket_t* raw);

		/**
		 * @brief Construct a socket TCP client.
		 * @param addr Remote address.
		 * @param port Remote port.
		 */
		explicit SocketTcpClient(const IPAddress& addr, uint16_t port);

		/**
		 * @brief Construct a socket TCP client.
		 * @param host Remote host.
		 * @param port Remote port.
		 */
		explicit SocketTcpClient(const String& host, uint16_t port);
		explicit SocketTcpClient(const SocketTcpClient& other) ; //!< Copy constructor.
		explicit SocketTcpClient(SocketTcpClient&& other) noexcept ; //!< Move constructor.
		~SocketTcpClient() override; //!< Socket TCP destructor.

		SocketTcpClient& operator =(const SocketTcpClient& client); //!< Copy assignment.
		SocketTcpClient& operator =( SocketTcpClient&& client) noexcept ; //!< Move assignment.

		bool operator ==(const SocketTcpClient& other); //!< Equality operator.
		bool operator !=(const SocketTcpClient& other); //!< Equality operator.

		explicit operator bool() const override;
		bool connected() const override; //!< Connection check.

		size_t available() const override; //!< Check for available data.

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
		 * @brief Set the socket timeout.
		 * @param seconds Timeout value in seconds.
		 */
		void setTimeout(time_t seconds) override;

		void close() override; //!< Close the TCP socket.

	private:
		socket_t *_socket;

		bool connect();
	};
}
