/*
 * ESP32 device test.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <lwiot.h>

#include <lwiot/thread.h>
#include <lwiot/log.h>
#include <lwiot/string.h>
#include <lwiot/watchdog.h>
#include <lwiot/datetime.h>
#include <lwiot/application.h>

#include <lwiot/network/stdnet.h>
#include <lwiot/network/wifiaccesspoint.h>
#include <lwiot/network/tcpclient.h>

static const char *cert =
		"-----BEGIN CERTIFICATE-----\n" \
  "MIIDjDCCAnSgAwIBAgIJALeLmnGMkGgXMA0GCSqGSIb3DQEBCwUAMFsxCzAJBgNV\n" \
  "BAYTAk5MMRYwFAYDVQQIDA1Ob29yZC1CcmFiYW50MQ4wDAYDVQQHDAVCcmVkYTEO\n" \
  "MAwGA1UECgwFbHdJb1QxFDASBgNVBAMMC2x3aW90LmxvY2FsMB4XDTE4MTIzMDE1\n" \
  "MzAxNFoXDTI4MTIyNzE1MzAxNFowWzELMAkGA1UEBhMCTkwxFjAUBgNVBAgMDU5v\n" \
  "b3JkLUJyYWJhbnQxDjAMBgNVBAcMBUJyZWRhMQ4wDAYDVQQKDAVsd0lvVDEUMBIG\n" \
  "A1UEAwwLbHdpb3QubG9jYWwwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIB\n" \
  "AQCoTOZJd1rbu883vCmV3zK+VCwpFmzoO/yrnEKzOudU3Mf3pdfA0Fr4x51TyKK+\n" \
  "QumnD3j0C3ZwCEfoQTo/eiOKnRKyNcKM/wfMgDZsvaENSCINomrpvFjkMMKb0XuQ\n" \
  "swefOOTlp7kmBTpxi7wora3EhU3b1wNm0EDY8c71bj4MwtY+nqqyKRDOaFRzNhy0\n" \
  "ulr23XJ6iJI8Caq61h2K0t//IJT8pc0F61/FLXrYZr1Yw1f1GG9Ecr5zvJJQgfqN\n" \
  "YjmuMhQwmoDNkhWYKs5y87ywO1cWR/xmJITVlv4XKaHY5KQGO1lnBu8GLka+/1xi\n" \
  "K+MOVlO025DfiiSDu75/lAHvAgMBAAGjUzBRMB0GA1UdDgQWBBQIap4O5ELEDHUS\n" \
  "jpouinxRWFUXJTAfBgNVHSMEGDAWgBQIap4O5ELEDHUSjpouinxRWFUXJTAPBgNV\n" \
  "HRMBAf8EBTADAQH/MA0GCSqGSIb3DQEBCwUAA4IBAQCOhyfvnHRVAoH2Tu5sE5SX\n" \
  "Rs3vdWooj3Sa+EhdunlAEcnsQ4VgZ6zGkUtxCuUgV3v+Mv6n1XbOPP/HjvTCFJnh\n" \
  "hd+ja4qF5qLD/RU0tgJMqVqgRH87cMMAhSzxYsrWF/Hg4fDtfR+qalTWGYfwgHAy\n" \
  "+M77thkzt1eKn2QosKNrvPp4xJH2MNC6M1zFH+cD2IEmBqRzZ0vOa6QnMRk7/K+0\n" \
  "8Wokv5E3nAs8hdTSy/Tr0MBv8RCrJH4Oyri83oF8zP7BV36NtPHIsyXmqOGfo7QC\n" \
  "JlBkySmXkFsDjma8VKoO17byGJffCLiTgudMe0O8x7RLaUZu09ujIyNs25sCVhsn\n" \
  "-----END CERTIFICATE-----";

class EspNetworkApplication : public lwiot::Functor {
public:
	explicit EspNetworkApplication() : _server(192,168,1,2)
	{
		char buf[] = "Hello, World!";

		this->_in = (char*)lwiot_mem_zalloc(sizeof(buf) + 1);
		this->_out = (char*)lwiot_mem_zalloc(sizeof(buf) + 1);

		memcpy(this->_out, (void*)buf, sizeof(buf));
		this->_size = sizeof(buf);
	}

	virtual ~EspNetworkApplication()
	{
		lwiot_mem_free(this->_in);
		lwiot_mem_free(this->_out);
	}

private:
	lwiot::IPAddress _server;
	char *_in;
	char *_out;
	size_t _size;

	void startAP(const lwiot::String& ssid, const lwiot::String& passw)
	{
		auto& ap = lwiot::WifiAccessPoint::instance();
		lwiot::IPAddress local(192, 168, 1, 1);
		lwiot::IPAddress subnet(255, 255, 255, 0);
		lwiot::IPAddress gw(192, 168, 1, 1);

		ap.start();
		ap.config(local, gw, subnet);
		ap.begin(ssid, passw, 4);
	}

	void test_sslclient()
	{
		ssl_context_t context = {0,0,0};
		secure_socket_t* socket;
		remote_addr_t remote;

		context.root_ca = cert;
		socket = secure_socket_create();

		assert(socket);

		this->_server.toRemoteAddress(remote);
		remote.port = to_netorders(5300);
		auto result = secure_socket_connect(socket, "lwiot.local", &remote, &context);

		if(!result) {
			print_dbg("Unable to connect!\n");
			return;
		}

		secure_socket_send(socket, this->_out, this->_size);
		secure_socket_recv(socket, this->_in, this->_size);
		print_dbg("Readback: %s\n", this->_in);
		secure_socket_close(socket);
	}

protected:
	void run() override
	{
		printf("Main thread started!\n");
		this->startAP("lwIoT test", "testap1234");

		while(true) {
			this->test_sslclient();
			print_dbg("PING!\n");
			lwiot_sleep(1000);
		}
	}
};

extern "C" void main_start(void)
{
	EspNetworkApplication runner;
	lwiot::Application app(runner);

	app.start();
}

