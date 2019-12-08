/*
 * UDP server wrapper class.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file socketudpserver.h

#pragma once

#include <lwiot/network/stdnet.h>
#include <lwiot/network/udpclient.h>
#include <lwiot/network/udpserver.h>
#include <lwiot/network/socketudpserver.h>

namespace lwiot
{
	/**
	 * @brief UDP server socket wrapper.
	 */
	class SocketUdpServer : public UdpServer {
	public:
		explicit SocketUdpServer(); //!< Construct a server socket.

		/**
		 * @brief Construct a server socket.
		 * @param addr Bind address.
		 * @param port Bind port.
		 */
		explicit SocketUdpServer(bind_addr_t addr, uint16_t port);

		/**
		 * @brief Construct a server socket.
		 * @param addr Bind address.
		 * @param port Bind port.
		 */
		explicit SocketUdpServer(const IPAddress& addr, uint16_t port);
		~SocketUdpServer() override; //!< Destroy a UDP server socket.

		void close() override; //!< Close the underlying socket.
		bool bind() override; //!< Bind to the bind address & port.

		/**
		 * @brief Bind to \p addr and \p port.
		 * @param addr Bind address.
		 * @param port Bind port.
		 * @return A success indicator.
		 */
		bool bind(bind_addr_t addr, uint16_t port) override;

		/**
		 * @brief Bind to \p addr and \p port.
		 * @param addr Bind address.
		 * @param port Bind port.
		 * @return A success indicator.
		 */
		bool bind(const IPAddress& addr, uint16_t port) override;

		/**
		 * @brief Receive data.
		 * @param buffer Receive buffer.
		 * @param length Length of \p buffer.
		 * @return The remote client.
		 */
		UniquePointer<UdpClient> recv(void *buffer, size_t& length) override;

		/**
		 * @brief Set the server socket timeout.
		 * @param tmo Timeout in seconds.
		 */
		void setTimeout(int tmo) override;

	private:
		socket_t* _socket;
	};
}
