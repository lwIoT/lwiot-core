/*
 * Secure network implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include "mbedtls.h"

#include <stdlib.h>
#include <lwiot.h>
#include <assert.h>
#include <unistd.h>

#include <lwiot/types.h>
#include <lwiot/log.h>

#include <lwiot/network/stdnet.h>

#ifdef HAVE_LWIP

#include <lwip/sockets.h>
#include <lwip/inet.h>
#include <lwip/ip_addr.h>

#else
#include <fcntl.h>
#include <netinet/tcp.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#endif

const char *pers = "lwiot-tls";

static int handle_error(int err)
{
	if(err == -30848){
		return err;
	}

#ifdef MBEDTLS_ERROR_C
	char error_buf[100];
	mbedtls_strerror(err, error_buf, 100);
	print_dbg("%s\n", error_buf);
#endif

	print_dbg("MbedTLS message code: %d\n", err);
	return err;
}

static int ip4_connect(remote_addr_t* addr)
{
	struct sockaddr_in sockaddr;
	int sockfd;

	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = addr->port;
	sockaddr.sin_addr.s_addr = addr->addr.ip4_addr.ip;
	bzero(&sockaddr.sin_zero, sizeof(sockaddr.sin_zero));

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
		return sockfd;

	if(connect(sockfd, (struct sockaddr*) &sockaddr, sizeof(sockaddr)) < 0) {
		close(sockfd);
		return sockfd;
	}

	return sockfd;
}

static int ip6_connect(remote_addr_t* addr)
{
	struct sockaddr_in6 ip;
	int sockfd;

	assert(addr);
	sockfd = socket(PF_INET6, SOCK_STREAM, 0);
	if(sockfd < 0)
		return false;

#ifdef HAVE_LWIP
	memcpy(ip.sin6_addr.un.u8_addr, addr->addr.ip6_addr.ip, sizeof(ip.sin6_addr.un.u8_addr));
	ip.sin6_len = sizeof(ip);
#else
	memcpy(ip.sin6_addr.__in6_u.__u6_addr8, addr->addr.ip6_addr.ip, sizeof(ip.sin6_addr.__in6_u.__u6_addr8));
#endif
	ip.sin6_family = AF_INET6;
	ip.sin6_port = addr->port;

	if(connect(sockfd, (const struct sockaddr*)&ip, sizeof(ip)) < 0) {
		close(sockfd);
		return sockfd;
	}

	return sockfd;
}

static void stop_ssl_socket(secure_socket_t *ssl_client, const char *rootCABuff,
							const char *cli_cert, const char *cli_key)
{
	print_dbg("Deleting SSL connection\n");

	if(ssl_client->socket >= 0) {
		close(ssl_client->socket);
		ssl_client->socket = -1;
	}

	mbedtls_ssl_free(&ssl_client->ssl_ctx);
	mbedtls_ssl_config_free(&ssl_client->ssl_conf);
	mbedtls_ctr_drbg_free(&ssl_client->drbg_ctx);
	mbedtls_entropy_free(&ssl_client->entropy_ctx);
}

static int start_ssl_client(secure_socket_t *ssl_client, remote_addr_t* remote, const char *host,
					const char *rootCABuff, const char *cli_cert, const char *cli_key)
{
	char buf[512];
	int ret, flags, timeout;
	int enable = 1;

	print_dbg("Starting socket..\n");

	if(remote->version == 6) {
		ssl_client->socket = ip6_connect(remote);
	} else {
		ssl_client->socket = ip4_connect(remote);
	}

	if(ssl_client->socket < 0) {
		print_dbg("ERROR opening socket\n");
		return ssl_client->socket;
	}

	timeout = 30000;
#ifdef HAVE_LWIP
	lwip_setsockopt(ssl_client->socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
	lwip_setsockopt(ssl_client->socket, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
	lwip_setsockopt(ssl_client->socket, IPPROTO_TCP, TCP_NODELAY, &enable, sizeof(enable));
	lwip_setsockopt(ssl_client->socket, SOL_SOCKET, SO_KEEPALIVE, &enable, sizeof(enable));
#else
	setsockopt(ssl_client->socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
	setsockopt(ssl_client->socket, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
	setsockopt(ssl_client->socket, IPPROTO_TCP, TCP_NODELAY, &enable, sizeof(enable));
	setsockopt(ssl_client->socket, SOL_SOCKET, SO_KEEPALIVE, &enable, sizeof(enable));
#endif

	//fcntl(ssl_client->socket, F_SETFL, fcntl(ssl_client->socket, F_GETFL, 0) | O_NONBLOCK);

	print_dbg("Seeding the random number generator\n");
	mbedtls_entropy_init(&ssl_client->entropy_ctx);

	ret = mbedtls_ctr_drbg_seed(&ssl_client->drbg_ctx, mbedtls_entropy_func,
	                            &ssl_client->entropy_ctx, (const unsigned char *) pers, strlen(pers));
	if(ret < 0) {
		return handle_error(ret);
	}

	print_dbg("Setting up the SSL/TLS structure..\n");

	if((
			   ret = mbedtls_ssl_config_defaults(&ssl_client->ssl_conf,
			                                     MBEDTLS_SSL_IS_CLIENT,
			                                     MBEDTLS_SSL_TRANSPORT_STREAM,
			                                     MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
		return handle_error(ret);
	}

	// MBEDTLS_SSL_VERIFY_REQUIRED if a CA certificate is defined on Arduino IDE and
	// MBEDTLS_SSL_VERIFY_NONE if not.

	if(rootCABuff != NULL) {
		print_dbg("Loading CA cert\n");
		mbedtls_x509_crt_init(&ssl_client->ca_cert);
		mbedtls_ssl_conf_authmode(&ssl_client->ssl_conf, MBEDTLS_SSL_VERIFY_REQUIRED);
		ret = mbedtls_x509_crt_parse(&ssl_client->ca_cert, (const unsigned char *) rootCABuff, strlen(rootCABuff) + 1);
		mbedtls_ssl_conf_ca_chain(&ssl_client->ssl_conf, &ssl_client->ca_cert, NULL);

		if(ret < 0) {
			return handle_error(ret);
		}
	} else {
		mbedtls_ssl_conf_authmode(&ssl_client->ssl_conf, MBEDTLS_SSL_VERIFY_NONE);
	}

	if(cli_cert != NULL && cli_key != NULL) {
		mbedtls_x509_crt_init(&ssl_client->client_cert);
		mbedtls_pk_init(&ssl_client->client_key);

		print_dbg("Loading CRT cert\n");

		ret = mbedtls_x509_crt_parse(&ssl_client->client_cert, (const unsigned char *) cli_cert, strlen(cli_cert) + 1);
		if(ret < 0) {
			return handle_error(ret);
		}

		print_dbg("Loading private key\n");
		ret = mbedtls_pk_parse_key(&ssl_client->client_key, (const unsigned char *) cli_key, strlen(cli_key) + 1, NULL,
		                           0);

		if(ret != 0) {
			return handle_error(ret);
		}

		mbedtls_ssl_conf_own_cert(&ssl_client->ssl_conf, &ssl_client->client_cert, &ssl_client->client_key);
	}

	print_dbg("Setting hostname for TLS session..\n");

	// Hostname set here should match CN in server certificate
	if((ret = mbedtls_ssl_set_hostname(&ssl_client->ssl_ctx, host)) != 0) {
		return handle_error(ret);
	}

	mbedtls_ssl_conf_rng(&ssl_client->ssl_conf, mbedtls_ctr_drbg_random, &ssl_client->drbg_ctx);

	if((ret = mbedtls_ssl_setup(&ssl_client->ssl_ctx, &ssl_client->ssl_conf)) != 0) {
		return handle_error(ret);
	}

	mbedtls_ssl_set_bio(&ssl_client->ssl_ctx, &ssl_client->socket, mbedtls_net_send, mbedtls_net_recv, NULL);

	print_dbg("Performing the SSL/TLS handshake..\n");
	unsigned long handshake_start_time = lwiot_tick_ms();
	while((ret = mbedtls_ssl_handshake(&ssl_client->ssl_ctx)) != 0) {
		if(ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
			return handle_error(ret);
		}
		if((lwiot_tick_ms() - handshake_start_time) > ssl_client->handshake_timeout)
			return -1;

		lwiot_sleep(10);
	}


	if(cli_cert != NULL && cli_key != NULL) {
		print_dbg("Protocol is %s Ciphersuite is %s\n", mbedtls_ssl_get_version(&ssl_client->ssl_ctx),
		      mbedtls_ssl_get_ciphersuite(&ssl_client->ssl_ctx));
		if((ret = mbedtls_ssl_get_record_expansion(&ssl_client->ssl_ctx)) >= 0) {
			print_dbg("Record expansion is %d\n", ret);
		} else {
			print_dbg("Record expansion is unknown (compression)\n");
		}
	}

	print_dbg("Verifying peer X.509 certificate..\n");

	if((flags = mbedtls_ssl_get_verify_result(&ssl_client->ssl_ctx)) != 0) {
		bzero(buf, sizeof(buf));
		mbedtls_x509_crt_verify_info(buf, sizeof(buf), "  ! ", flags);
		print_dbg("Failed to verify peer certificate! verification info: %s\n", buf);
		stop_ssl_socket(ssl_client, rootCABuff, cli_cert, cli_key);  //It's not safe continue.
		return handle_error(ret);
	} else {
		print_dbg("Certificate verified..\n");
	}

	if(rootCABuff != NULL) {
		mbedtls_x509_crt_free(&ssl_client->ca_cert);
	}

	if(cli_cert != NULL) {
		mbedtls_x509_crt_free(&ssl_client->client_cert);
	}

	if(cli_key != NULL) {
		mbedtls_pk_free(&ssl_client->client_key);
	}

	return ssl_client->socket;
}

bool secure_socket_connect(secure_socket_t *socket, const char *host, remote_addr_t* addr, ssl_context_t* context)
{
	int fd = start_ssl_client(socket, addr, host, context->root_ca, context->client_cert, context->client_key);
	return fd >= 0;
}

secure_socket_t* secure_socket_create()
{
	secure_socket_t* ssl_client;

	ssl_client = lwiot_mem_zalloc(sizeof(*ssl_client));
	mbedtls_ssl_init(&ssl_client->ssl_ctx);
	mbedtls_ssl_config_init(&ssl_client->ssl_conf);
	mbedtls_ctr_drbg_init(&ssl_client->drbg_ctx);
	ssl_client->handshake_timeout = 3000;

	return ssl_client;
}

void secure_socket_close(secure_socket_t* ssl_client)
{
	assert(ssl_client);
	if(ssl_client->socket >= 0) {
		close(ssl_client->socket);
		ssl_client->socket = -1;
	}

	mbedtls_ssl_free(&ssl_client->ssl_ctx);
	mbedtls_ssl_config_free(&ssl_client->ssl_conf);
	mbedtls_ctr_drbg_free(&ssl_client->drbg_ctx);
	mbedtls_entropy_free(&ssl_client->entropy_ctx);

	lwiot_mem_free(ssl_client);
}

size_t secure_socket_available(secure_socket_t* socket)
{
	int ret;
	size_t res;

	ret = mbedtls_ssl_read(&socket->ssl_ctx, NULL, 0);
	res = mbedtls_ssl_get_bytes_avail(&socket->ssl_ctx);

	if(ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE && ret < 0)
		return 0UL;

	return res;
}

ssize_t secure_socket_send(secure_socket_t* ssl_client, const void *data, size_t length)
{
	ssize_t ret;

	while ((ret = mbedtls_ssl_write(&ssl_client->ssl_ctx, data, length)) <= 0) {
		if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
			return handle_error(ret);
		}
	}

	return ret;
}

ssize_t secure_socket_recv(secure_socket_t* socket, void *data, size_t length)
{
	ssize_t ret;

	while ((ret = mbedtls_ssl_read(&socket->ssl_ctx, data, length)) <= 0) {
		if(ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE)
			return handle_error(ret);
	}

	return ret;
}
