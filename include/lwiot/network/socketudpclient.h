/*
 * UDP client wrapper class.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

namespace lwiot
{
	class SocketUdpClient : public UdpClient {
	public:
		explicit SocketUdpClient();
		explicit SocketUdpClient(const IPAddress& addr, uint16_t port, socket_t* srv = nullptr);
		explicit SocketUdpClient(const String& host, uint16_t port);
		~SocketUdpClient() override;

		void close() override;
		size_t available() const override;

		using UdpClient::read;
		using UdpClient::write;

		ssize_t read(void *output, const size_t &length) override;
		ssize_t write(const void *bytes, const size_t& length) override;

	private:
		socket_t* _socket;
		bool _noclose;

		void init();
	};
}
