/*
 * TCP server wrapper
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file sockettcpserver.h

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/log.h>
#include <lwiot/stl/string.h>
#include <lwiot/stream.h>

#include <lwiot/network/ipaddress.h>
#include <lwiot/network/tcpclient.h>
#include <lwiot/network/stdnet.h>
#include <lwiot/network/tcpserver.h>
#include <lwiot/uniquepointer.h>

namespace lwiot
{
	/**
	 * @brief Server socket wrapper.
	 * @ingroup net
	 */
	class SocketTcpServer : public TcpServer {
	public:
		explicit SocketTcpServer(); //!< Construct a new socket TCP server.
		/**
		 * @brief Construct a new socket TCP server.
		 * @param addr Bind address.
		 * @param port Bind port.
		 */
		explicit SocketTcpServer(BindAddress addr, uint16_t port);

		/**
		 * @brief Construct a new socket TCP server.
		 * @param addr Remote address.
		 * @param port Remote port.
		 */
		explicit SocketTcpServer(const IPAddress& addr, uint16_t port);

		SocketTcpServer(SocketTcpServer&& server) noexcept ; //!< Move constructor.
		~SocketTcpServer() override; //!< Destroy a socket TCP server.

		SocketTcpServer& operator =(const SocketTcpServer& other); //!< Copy assignment.
		SocketTcpServer& operator =(SocketTcpServer&& other) noexcept ; //!< Move assignment.

		bool operator ==(const SocketTcpServer& other); //!< Equality operator.
		bool operator !=(const SocketTcpServer& other); //!< Equality operator.

		bool bind() const override; //!< Bind the socket server.

		/**
		 * @brief Bind the socket server.
		 * @param addr Bind address.
		 * @param port Bind port.
		 * @return Success indicator.
		 */
		bool bind(BindAddress addr, uint16_t port);

		/**
		 * @brief Bind the socket server.
		 * @param addr Bind address.
		 * @param port Bind port.
		 * @return Success indicator.
		 */
		bool bind(const IPAddress& addr, uint16_t port) override;

		void connect() override; //!< Connect the server socket.
		UniquePointer<TcpClient> accept() override; //!< Accept a TCP client.
		void close() override; //!< Close the server socket.
		void setTimeout(time_t seconds) override ; //!< Set the server socket timeout.

#ifdef HAVE_LWIP
		static constexpr int BacklogSize = 16;
#else
		static constexpr int BacklogSize = 128;
#endif

	private:
		socket_t *_socket;
	};
}
