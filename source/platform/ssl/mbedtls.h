/*
 * Secure network definitions.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <mbedtls/platform.h>
#include <mbedtls/net_sockets.h>
#include <mbedtls/debug.h>
#include <mbedtls/ssl.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/error.h>

#define HAVE_SECURE_SOCKET_DEFINITION 1

typedef struct sslclient_context {
	int socket;
	mbedtls_ssl_context ssl_ctx;
	mbedtls_ssl_config ssl_conf;

	mbedtls_ctr_drbg_context drbg_ctx;
	mbedtls_entropy_context entropy_ctx;

	mbedtls_x509_crt ca_cert;
	mbedtls_x509_crt client_cert;
	mbedtls_pk_context client_key;

	unsigned long handshake_timeout;
} secure_socket_t;

