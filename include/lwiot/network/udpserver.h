/*
 * UDP server wrapper class.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file udpserver.h

#pragma once

#include <lwiot/network/ipaddress.h>
#include <lwiot/uniquepointer.h>

namespace lwiot
{
	/**
	 * @brief UDP server wrapper.
	 */
	class UdpServer {
	public:
		explicit UdpServer(); //!< Construct a UDP server.

		/**
		 * @brief UDP server wrapper.
		 * @param addr Bind address.
		 * @param port Local port.
		 */
		explicit UdpServer(const IPAddress& addr, uint16_t port);
		virtual ~UdpServer() = default; //!< Destroy a UDP server.

		virtual void close() = 0; //!< Close the UDP server socket.
		virtual bool bind() = 0; //!< Bind the UDP server.
		virtual bool bind(bind_addr_t addr, uint16_t port) = 0; //!< Bind the UDP server.
		virtual bool bind(const IPAddress& addr, uint16_t port); //!< Bind the UDP server.

		/**
		 * @brief Receive data from the remote.
		 * @param buffer Input buffer.
		 * @param length Length of \p buffer.
		 * @return The remote client.
		 */
		virtual UniquePointer<UdpClient> recv(void *buffer, size_t& length) = 0;

		/**
		 * @brief Set the timeout of the server socket in seconds.
		 * @param tmo Timeout in seconds.
		 */
		virtual void setTimeout(int tmo) = 0;

		const IPAddress& address() const; //!< Get the remote address.
		uint16_t port() const; //!< Get the remote port.

	private:
		IPAddress _bind_addr;
		uint16_t _bind_port;
	};
}
