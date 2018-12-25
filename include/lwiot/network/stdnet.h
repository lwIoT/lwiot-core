/*
 * Standard network definitions.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot.h>
#include <stdlib.h>

#include <lwiot/types.h>
#include <lwiot/error.h>

#ifndef HAVE_SOCKET_DEFINITION
typedef long socket_t;
#endif

static inline uint32_t lwiot_bswap_32 (uint32_t __bsx)
{
	return __builtin_bswap32 (__bsx);
}

#define _HTONS(n) (((((unsigned short)(n) & 0xFF)) << 8) | (((unsigned short)(n) & 0xFF00) >> 8))

static inline uint32_t to_netorderl(uint32_t ip)
{
#ifdef HAVE_BIG_ENDIAN
	return ip;
#else
	return lwiot_bswap_32(ip);
#endif
}

static inline uint32_t to_netorders(uint16_t s)
{
#ifdef HAVE_BIG_ENDIAN
	return s;
#else
	return _HTONS(s);
#endif
}

#define to_hostorders(_x) to_netorders(_x)

typedef struct {
	union {
		struct {
			uint32_t ip;
		} ip4_addr;
		struct {
			uint8_t ip[16];
		} ip6_addr;
	} addr;

	uint16_t port;
	int8_t version;
} remote_addr_t;

typedef enum {
	SOCKET_STREAM,
	SOCKET_DGRAM
} socket_type_t;

typedef enum {
	BIND_ADDR_ANY,
	BIND_ADDR_LB,

	BIND6_ADDR_ANY,
	BIND6_ADDR_LB
} bind_addr_t;

CDECL
extern DLL_EXPORT socket_t* tcp_socket_create(remote_addr_t* remote);
extern DLL_EXPORT ssize_t tcp_socket_send(socket_t* socket, const void* data, size_t length);
extern DLL_EXPORT ssize_t tcp_socket_read(socket_t* socket, void* data, size_t length);
extern DLL_EXPORT size_t tcp_socket_available(socket_t* socket);
extern DLL_EXPORT socket_t* udp_socket_create(remote_addr_t* remote);
extern DLL_EXPORT ssize_t udp_send_to(socket_t* socket, const void *data, size_t length, remote_addr_t* remote);
extern DLL_EXPORT ssize_t udp_recv_from(socket_t* socket, void *data, size_t length, remote_addr_t* remote);

extern DLL_EXPORT void socket_close(socket_t* socket);
extern DLL_EXPORT void socket_set_timeout(socket_t *sock, int tmo);

/* SERVER OPS */
extern DLL_EXPORT socket_t* server_socket_create(socket_type_t type, bool ipv6);
extern DLL_EXPORT bool server_socket_bind(socket_t* sock, bind_addr_t addr, uint16_t port);
extern DLL_EXPORT bool server_socket_listen(socket_t *socket);
extern DLL_EXPORT socket_t* server_socket_accept(socket_t* socket);

/* DNS */
extern DLL_EXPORT int dns_resolve_host(const char *host, remote_addr_t* addr);
CDECL_END
