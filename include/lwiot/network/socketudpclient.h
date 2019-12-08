/*
 * UDP client wrapper class.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file socketudpclient.h

namespace lwiot
{
	/**
	 * @brief UDP socket wrapper.
	 */
	class SocketUdpClient : public UdpClient {
	public:
		explicit SocketUdpClient(); //!< Construct a new UDP socket.

		/**
		 * @brief Construct a new UDP client.
		 * @param addr Remote address.
		 * @param port Remote port.
		 * @param srv Raw server socket object.
		 */
		explicit SocketUdpClient(const IPAddress& addr, uint16_t port, socket_t* srv = nullptr);

		/**
		 * @brief Construct a new UDP client.
		 * @param host Remote address.
		 * @param port Remote port.
		 */
		explicit SocketUdpClient(const String& host, uint16_t port);
		explicit SocketUdpClient(const SocketUdpClient&) = delete;
		~SocketUdpClient() override; //!< Destroy a UDP client.

		SocketUdpClient& operator=(const SocketUdpClient&) = delete;

		void close() override; //!< Close a UDP client.
		size_t available() const override; //!< Check for available data.

		void begin() override ; //!< Start the UDP client socket.

		/**
		 * @brief Start the UDP client socket.
		 * @param host Remote host.
		 * @param port Remote port.
		 */
		void begin(const stl::String& host, uint16_t port) override ;

		/**
		 * @brief Start the UDP client socket.
		 * @param addr Remote host.
		 * @param port Remote port.
		 */
		void begin(const IPAddress& addr, uint16_t port) override ;

		using UdpClient::read;
		using UdpClient::write;

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
		 * @brief Set the socket timeout.
		 * @param seconds Timeout value in seconds.
		 */
		void setTimeout(time_t seconds) override;

	private:
		socket_t* _socket;
		bool _noclose;

		void init();
	};
}
