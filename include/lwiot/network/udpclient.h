/*
 * UDP client wrapper class.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file udpclient.h

#pragma once

#include <lwiot/types.h>
#include <lwiot/stream.h>

#include <lwiot/network/ipaddress.h>

namespace lwiot
{
	/**
	 * @brief UDP client wrapper.
	 */
	class UdpClient : public Stream {
	public:
		explicit UdpClient(); //!< Construct a new UDP client.

		/**
		 * @brief UDP client constructor.
		 * @param addr Remote address.
		 * @param port Remote port.
		 */
		explicit UdpClient(const IPAddress& addr, uint16_t port);

		/**
		 * @brief UDP client constructor.
		 * @param host Remote address.
		 * @param port Remote port.
		 */
		explicit UdpClient(const String& host, uint16_t port);
		~UdpClient() override = default; //!< Destroy a UDP client.

		virtual void begin() = 0; //!< Start a UDP client.
		virtual void begin(const stl::String& host, uint16_t port) = 0; //!< Start a UDP client.
		virtual void begin(const IPAddress& addr, uint16_t port) = 0; //!< Start a UDP client.

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

		virtual void close() = 0; //!< Close a UDP socket/client.

		using Stream::read;
		uint8_t read() override; //!< Read from the client.

		using Stream::write;
		bool write(uint8_t byte) override; //!< Write a byte to the client.

		const IPAddress& address() const; //!< Get the remote address.
		uint16_t port() const; //!< Get the remote port.

		/**
		 * @brief Resole the remote host.
		 * @note The remote host must be set using the constructor or UdpClient::begin.
		 */
		void resolve();

	protected:
		IPAddress _remote;
		uint16_t _port;
		stl::String _host;
	};
}
