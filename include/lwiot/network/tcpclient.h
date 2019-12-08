/*
 * TCP client wrapper.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file tcpclient.h

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
	/**
	 * @brief TCP client wrapper.
	 * @ingroup net
	 */
	class TcpClient : public Stream {
	public:
		explicit TcpClient();
		explicit TcpClient(const IPAddress& addr, uint16_t port); //!< Construct a new TCP client.
		explicit TcpClient(const String& host, uint16_t port); //!< Construct a new TCP client.
		TcpClient(const TcpClient& client) = default; //!< Construct a new TCP client.
		TcpClient( TcpClient&& client) noexcept = default; //!< Construct a new TCP client.
		~TcpClient() override = default; //!< Destroy a TCP client.

		TcpClient& operator =(const TcpClient& client); //!< Copy assignment operator.
		TcpClient& operator =(TcpClient&& client) noexcept ; //!< Move assignment operator.

		virtual bool operator ==(const TcpClient& other); //!< Equality operator.
		virtual bool operator !=(const TcpClient& other); //!< Equality operator.

		virtual explicit operator bool() const = 0; //!< Boolean conversion operator.
		virtual bool connected() const = 0; //!< Connection check.

		using Stream::available;

		/**
		 * @name Stream operators
		 * @{
		 */

		Stream &operator<<(char x) override; //!< Stream operator.
		Stream &operator<<(short x) override; //!< Stream operator.
		Stream &operator<<(int x) override; //!< Stream operator.
		Stream &operator<<(const long &x) override; //!< Stream operator.
		Stream &operator<<(const long long &x) override; //!< Stream operator.
		Stream &operator<<(unsigned char x) override; //!< Stream operator.
		Stream &operator<<(unsigned short x) override; //!< Stream operator.
		Stream &operator<<(unsigned int x) override; //!< Stream operator.
		Stream &operator<<(const unsigned long &x) override; //!< Stream operator.
		Stream &operator<<(const unsigned long long &x) override; //!< Stream operator.
		Stream &operator<<(const double &x) override; //!< Stream operator.
		Stream &operator<<(const float &x) override; //!< Stream operator.
		Stream &operator<<(const String &str) override; //!< Stream operator.
		Stream &operator<<(const char *cstr) override; //!< Stream operator.

		/** @} */

		using Stream::read;
		uint8_t read() override; //!< Read a byte.

		using Stream::write;
		bool write(uint8_t byte) override; //!< Write a byte.

		/**
		 * @brief Connect to the remote end.
		 * @param addr Remote address.
		 * @param port Remote port.
		 * @return Success indicator.
		 */
		virtual bool connect(const IPAddress& addr, uint16_t port) = 0;

		/**
		 * @brief Connect to the remote end.
		 * @param host Remote address.
		 * @param port Remote port.
		 * @return Success indicator.
		 */
		virtual bool connect(const String& host, uint16_t port)    = 0;

		using Stream::setTimeout;

		virtual void close() = 0; //!< Close a TCP socket.

		const IPAddress& remote() const; //!< Get the remote address.
		uint16_t port() const; //!< Get the remote port.

	protected:
		IPAddress _remote_addr; //!< Get the remote address.
		uint16_t _remote_port;  //!< Get the remote port.
	};
}
