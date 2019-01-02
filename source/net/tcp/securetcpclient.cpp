/*
 * TLS TCP client wrapper.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <assert.h>
#include <lwiot.h>

#include <lwiot/log.h>
#include <lwiot/types.h>

#include <lwiot/network/tcpclient.h>
#include <lwiot/network/stdnet.h>
#include <lwiot/network/securetcpclient.h>

namespace lwiot
{
	SecureTcpClient::SecureTcpClient() : TcpClient(), _socket(nullptr), _host("")
	{
	}

	SecureTcpClient::SecureTcpClient(const lwiot::IPAddress &addr, uint16_t port, const String& host) :
		TcpClient(addr, port), _socket(nullptr), _host(host)
	{
	}

	SecureTcpClient::SecureTcpClient(const lwiot::SecureTcpClient &other) :
		TcpClient(other.remote(), other.port()), _socket(nullptr), _host(other._host)
	{
	}

	SecureTcpClient::SecureTcpClient(lwiot::SecureTcpClient &&other) :
		TcpClient(other.remote(), other.port()), _socket(other._socket), _host(other._host)
	{
		other._socket = nullptr;
		other._remote_port = 0;
		other._remote_addr = IPAddress();
		other._host = "";
	}

	SecureTcpClient::~SecureTcpClient()
	{
		this->close();
	}

	SecureTcpClient& SecureTcpClient::operator=(const lwiot::SecureTcpClient &client)
	{
		if(this->connected()) {
			this->close();
		}

		this->_host = client._host;
		this->connect(client.remote(), client.port());
		return *this;
	}

	SecureTcpClient& SecureTcpClient::operator=(lwiot::SecureTcpClient &&other)
	{
		if(this->connected()) {
			this->close();
		}

		this->_host = other._host;
		this->_remote_addr = other.remote();
		this->_remote_port = other.port();
		this->_socket = other._socket;

		other._socket = nullptr;
		other._remote_port = 0;
		other._remote_addr = IPAddress();
		other._host = "";

		return *this;
	}

	bool SecureTcpClient::connect(const lwiot::String &host, uint16_t port)
	{
		const char *hname;
		remote_addr_t remote;

		hname = host.c_str();
		if(dns_resolve_host(hname, &remote) != -EOK)
			return false;

		this->setServerName(host);
		this->_remote_port = to_netorders(port);
		this->_remote_addr = IPAddress(remote);

		return this->connect();
	}

	bool SecureTcpClient::connect(const lwiot::IPAddress &addr, uint16_t port)
	{
		this->_remote_addr = addr;
		this->_remote_port = port;

		return this->connect();
	}

	bool SecureTcpClient::connect()
	{
		ssl_context_t context = {0,0,0};
		remote_addr_t remote;

		this->remote().toRemoteAddress(remote);
		remote.port = this->port();

		context.root_ca = this->_cert.c_str();

		this->_socket = secure_socket_create();
		assert(this->_socket);
		auto value = secure_socket_connect(this->_socket, this->_host.c_str(), &remote, &context);

		if(!value) {
			secure_socket_close(this->_socket);
			this->_socket = nullptr;
			return value;
		}

		return value;
	}

	void SecureTcpClient::setServerCertificate(const lwiot::String &cert)
	{
		this->_cert = cert;
	}

	void SecureTcpClient::setServerName(const lwiot::String &host)
	{
		this->_host = host;
	}

	void SecureTcpClient::close()
	{
		if(this->_socket == nullptr)
			return;

		secure_socket_close(this->_socket);
		this->_socket = nullptr;
	}

	ssize_t SecureTcpClient::read(void *output, const size_t &length)
	{
		assert(this->_socket);
		assert(output);

		return secure_socket_recv(this->_socket, output, length);
	}

	ssize_t SecureTcpClient::write(const void *bytes, const size_t &length)
	{
		assert(this->_socket);
		assert(bytes);

		return secure_socket_send(this->_socket, bytes, length);
	}

	size_t SecureTcpClient::available() const
	{
		assert(this->_socket);
		return secure_socket_available(this->_socket);
	}

	bool SecureTcpClient::connected() const
	{
		return this->_socket != nullptr;
	}

	SecureTcpClient::operator bool() const
	{
		return this->connected();
	}

	bool SecureTcpClient::operator==(const lwiot::SecureTcpClient &other)
	{
		if(this->_socket == other._socket)
			return true;

		return this->_host == other._host && this->_remote_addr == other._remote_addr &&
					this->_remote_port == other._remote_port;
	}

	bool SecureTcpClient::operator!=(const lwiot::SecureTcpClient &other)
	{
		return !(*this == other);
	}
}
