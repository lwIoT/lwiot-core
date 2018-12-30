/*
 * Generic TCP client unit test.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>
#include <assert.h>

#include <lwiot/log.h>
#include <lwiot/network/tcpclient.h>
#include <lwiot/network/sockettcpclient.h>
#include <lwiot/test.h>

#include <netinet/in.h>

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

static void test_sslclient()
{
	ssl_context_t context = {0};
	secure_socket_t* socket;
	remote_addr_t remote;
	lwiot::IPAddress server(127,0,0,1);

	context.root_ca = cert;
	socket = secure_socket_create();
	assert(socket);
	server.toRemoteAddress(remote);
	remote.port = htons(5300);
	auto result = secure_socket_connect(socket, "lwiot.local", &remote, &context);
	assert(result);

	char data[] = "Hello, World!";
	char readback[sizeof(data)];

	secure_socket_send(socket, data, strlen(data));
	secure_socket_recv(socket, (void*) readback, sizeof(readback));
	print_dbg("Readback: %s\n", readback);
	secure_socket_close(socket);
}

int main(int argc, char **argv)
{
	lwiot_init();
	test_sslclient();
	lwiot_destroy();
	wait_close();

	return -EXIT_SUCCESS;
}
