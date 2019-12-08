/*
 * TCP server wrapper
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file tcpserver.h

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/log.h>
#include <lwiot/stl/string.h>
#include <lwiot/stream.h>
#include <lwiot/network/ipaddress.h>
#include <lwiot/network/tcpclient.h>
#include <lwiot/uniquepointer.h>

namespace lwiot
{
	/**
	 * @brief TCP server.
	 */
	class TcpServer {
	public:
		explicit TcpServer(); //!< Construct a TCP server.

		/**
		 * @brief TCP server constructor.
		 * @param addr Bind address.
		 * @param port Bind port.
		 */
		explicit TcpServer(const IPAddress& addr, uint16_t port);
		virtual ~TcpServer() = default;

		TcpServer& operator =(const TcpServer& other); //!< Copy assignment operator.

		bool operator ==(const TcpServer& other); //!< Equality operator.
		bool operator !=(const TcpServer& other); //!< Equality operator.

		virtual bool bind() const = 0; //!< Bind the TCP server socket.
		virtual bool bind(const IPAddress& addr, uint16_t port); //!< Bind the TCP server socket.

		/**
		 * @brief Set the server socket timeout.
		 * @param seconds Timeout in seconds.
		 */
		virtual void setTimeout(time_t seconds) = 0;

		virtual void connect() = 0; //!< Connect the server socket.

		virtual UniquePointer<TcpClient> accept() = 0; //!< Accept a client.
		virtual void close() = 0; //!< Close the server.

		const IPAddress& address() const { return this->_bind_addr; } //!< Get the bind address.
		uint16_t port() const { return this->_bind_port; } //!< Get the bind port.

#ifdef HAVE_LWIP
		static constexpr int BacklogSize = 4;
#else
		static constexpr int BacklogSize = 128;
#endif

	protected:
		IPAddress _bind_addr; //!< Bind address.
		uint16_t _bind_port; //!< Bind port.
	};
}
