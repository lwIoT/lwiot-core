/*
 * Standard socket implementation for lwIP builds.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include "lwip_sockets.h"

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>
#include <unistd.h>
#include <assert.h>

#include <lwiot/types.h>
#include <lwiot/log.h>
#include <lwiot/error.h>
#include <lwiot/network/stdnet.h>

#include <lwip/sockets.h>
#include <lwip/inet.h>
#include <lwip/ip_addr.h>
#include <lwip/dns.h>

#define IP6_SIZE 16

#ifndef CONFIG_CLIENT_QUEUE_LENGTH
#define CONFIG_CLIENT_QUEUE_LENGTH 10
#endif

lwiot_event_t* lwiot_dns_event;

static bool ip4_connect(socket_t* sock, remote_addr_t* addr)
{
	struct sockaddr_in sockaddr;

	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = addr->port;
	sockaddr.sin_addr.s_addr = addr->addr.ip4_addr.ip;
	bzero(&sockaddr.sin_zero, sizeof(sockaddr.sin_zero));

	*sock = socket(AF_INET, SOCK_STREAM, 0);
	if(*sock < 0)
		return false;

	if(connect(*sock, (struct sockaddr*) &sockaddr, sizeof(sockaddr)) < 0) {
		close(*sock);
		return false;
	}

	return true;
}

static bool ip6_connect(socket_t* sock, remote_addr_t* addr)
{
	struct sockaddr_in6 ip;
	int sockfd;

	assert(sock);
	sockfd = socket(PF_INET6, SOCK_STREAM, 0);
	if(sockfd < 0)
		return false;

	*sock = sockfd;

	memcpy(ip.sin6_addr.un.u8_addr, addr->addr.ip6_addr.ip, sizeof(ip.sin6_addr.un.u8_addr));
	ip.sin6_len = sizeof(ip);
	ip.sin6_family = AF_INET6;
	ip.sin6_port = addr->port;

	if(connect(sockfd, (const struct sockaddr*)&ip, sizeof(ip)) < 0) {
		close(sockfd);
		return false;
	}

	return true;
}

static size_t socket_available(const socket_t* socket)
{
	int count;

	count = 0;
	assert(socket);
	ioctlsocket(*socket, FIONREAD, &count);

	return count;
}

size_t tcp_socket_available(socket_t* socket)
{
	return socket_available(socket);
}

size_t udp_socket_available(socket_t* socket)
{
	return socket_available(socket);
}

socket_t* tcp_socket_create(remote_addr_t* remote)
{
	socket_t *sock;
	bool result;

	sock = lwiot_mem_zalloc(sizeof(sock));
	assert(sock);

	if(remote->version == 6)
		result = ip6_connect(sock, remote);
	else
		result = ip4_connect(sock, remote);

	if(!result) {
		lwiot_mem_free(sock);
		return NULL;
	}

	return sock;
}

ssize_t tcp_socket_send(socket_t* socket, const void* data, size_t length)
{
	int fd;

	assert(socket);
	assert(data);

	fd = *socket;

	if(length == 0)
		return 0;

	return send(fd, data, length, 0);
}

ssize_t tcp_socket_read(socket_t* socket, void* data, size_t length)
{
	int fd;

	assert(socket);
	assert(data);

	fd = *socket;
	if(length == 0)
		return 0;

	return recv(fd, data, length, 0);
}

socket_t* udp_socket_create(remote_addr_t* remote)
{
	socket_t *sock;
	int fd;

	sock = lwiot_mem_zalloc(sizeof(sock));
	assert(sock);

	if(remote->version == 6) {
		fd = socket(PF_INET6, SOCK_DGRAM, 0);
	} else {
		fd = socket(PF_INET, SOCK_DGRAM, 0);
	}

	if(fd < 0) {
		lwiot_mem_free(sock);
		return NULL;
	}

	*sock = fd;
	return sock;
}

ssize_t udp_send_to(socket_t* socket, const void *data, size_t length, remote_addr_t* remote)
{
	struct sockaddr_in ip;
	struct sockaddr_in6 ip6;
	ssize_t rv;

	if(remote->version == 6) {
		memcpy(ip6.sin6_addr.un.u8_addr, remote->addr.ip6_addr.ip, IP6_SIZE);
		ip6.sin6_family = AF_INET6;
		ip6.sin6_port = remote->port;
		rv = sendto(*socket, data, length, 0, (struct sockaddr*)&ip6, sizeof(ip6));
	} else {
		ip.sin_family = AF_INET;
		ip.sin_port = remote->port;
		ip.sin_addr.s_addr = remote->addr.ip4_addr.ip;
		rv = sendto(*socket, data, length, 0, (struct sockaddr*)&ip, sizeof(ip));
	}

	return rv;
}

ssize_t udp_recv_from(socket_t* socket, void *data, size_t length, remote_addr_t* remote)
{
	struct sockaddr_in ip;
	struct sockaddr_in6 ip6;
	ssize_t rv;
	socklen_t socklen;

	if(remote->version == 6) {
		socklen = sizeof(ip6);
		rv = recvfrom(*socket, data, length, 0, (struct sockaddr*)&ip6, &socklen);

		if(rv < 0)
			return rv;

		remote->port = ip6.sin6_port;
		memcpy(remote->addr.ip6_addr.ip, ip6.sin6_addr.un.u8_addr, IP6_SIZE);
	} else {
		socklen = sizeof(ip);
		rv = recvfrom(*socket, data, length, 0, (struct sockaddr*)&ip, &socklen);

		if(rv < 0)
			return rv;

		remote->port = ip.sin_port;
		remote->addr.ip4_addr.ip = ip.sin_addr.s_addr;
	}

	return rv;
}

void socket_close(socket_t* socket)
{
	assert(socket);
	close(*socket);
	lwiot_mem_free(socket);
}

void socket_set_timeout(socket_t *sock, int tmo)
{
	assert(sock);
	setsockopt(*sock, SOL_SOCKET, SO_RCVTIMEO, (char *) &tmo, sizeof(int));
}

/* SERVER OPS */
socket_t* server_socket_create(socket_type_t type, bool ipv6)
{
	socket_t *sock;
	int fd;
	int domain;

	if(ipv6)
		domain = PF_INET6;
	else
		domain = PF_INET;

	if(type == SOCKET_DGRAM) {
		fd = socket(domain, SOCK_DGRAM, 0);
	} else {
		fd = socket(domain, SOCK_STREAM, 0);
	}

	if(fd < 0) {
		return NULL;
	}

	sock = lwiot_mem_zalloc(sizeof(sock));
	*sock = fd;

	return sock;
}

static bool bind_ipv4(const socket_t* sock, remote_addr_t* addr, uint16_t port)
{
	int fd;
	struct sockaddr_in server;

	fd = *sock;
	assert(fd >= 0);

	server.sin_port = port;
	server.sin_family = AF_INET;
	server.sin_len = sizeof(server);
	server.sin_addr.s_addr = addr->addr.ip4_addr.ip;

	return lwip_bind(fd, (struct sockaddr*)&server, sizeof(server)) < 0 ? false : true;
}

static bool bind_ipv6(const socket_t* sock, remote_addr_t* addr, uint16_t port)
{
	int fd;
	struct sockaddr_in6 server;

	fd = *sock;
	assert(fd >= 0);

	server.sin6_port = port;
	server.sin6_family = AF_INET6;
	server.sin6_len = sizeof(server);
	memcpy(server.sin6_addr.un.u8_addr, addr->addr.ip6_addr.ip, sizeof(addr->addr.ip6_addr.ip));

	return bind(fd, (struct sockaddr*)&server, sizeof(server)) < 0 ? false : true;
}

bool server_socket_bind_to(socket_t* sock, remote_addr_t* remote, uint16_t port)
{
	if(remote->version == 6) {
		return bind_ipv6(sock, remote, port);
	}

	return bind_ipv4(sock, remote, port);
}

bool server_socket_bind(socket_t* sock, bind_addr_t addr, uint16_t port)
{
	remote_addr_t remote;
	assert(sock);

	switch(addr) {
	default:
	case BIND_ADDR_ANY:
		remote.addr.ip4_addr.ip = INADDR_ANY;
		return bind_ipv4(sock, &remote, port);

	case BIND_ADDR_LB:
		remote.addr.ip4_addr.ip = INADDR_LOOPBACK;
		return bind_ipv4(sock, &remote, port);

	case BIND6_ADDR_ANY:
		memcpy(remote.addr.ip6_addr.ip, in6addr_any.un.u8_addr, sizeof(in6addr_any.un.u8_addr));
		return bind_ipv6(sock, &remote, port);
	}

	return false;
}

bool server_socket_listen(socket_t *socket)
{
	int rv;

	assert(socket);
	rv = listen(*socket, CONFIG_CLIENT_QUEUE_LENGTH);

	return rv < 0 ? false : true;
}

socket_t* server_socket_accept(socket_t* socket)
{
	int sock;
	socket_t *client;
	struct sockaddr addr;
	socklen_t length;

	assert(socket);
	sock = lwip_accept(*socket, &addr, &length);
	//sock = accept(*socket, NULL, NULL);

	if(sock < 0)
		return NULL;

	client = lwiot_mem_zalloc(sizeof(socket));
	assert(client);
	*client = sock;
	return client;
}

static void dns_host_found_cb(const char *cb, const ip_addr_t* ip, void *arg)
{
	remote_addr_t *addr;

	addr = arg;
	assert(arg);

	print_dbg("DNS host found CB!\n");

	if(ip->type == IPADDR_TYPE_V4 || ip->type == IPADDR_TYPE_ANY) {
		addr->version = 4;
		addr->addr.ip4_addr.ip = ip->u_addr.ip4.addr;
	} else {
		addr->version = 6;
		memcpy(addr->addr.ip6_addr.ip, ip->u_addr.ip6.addr, sizeof(addr->addr.ip6_addr.ip));
	}

	lwiot_event_signal(lwiot_dns_event);
}

/* DNS */
int dns_resolve_host(const char *host, remote_addr_t* addr)
{
	ip_addr_t ip;
	int rv;

	assert(lwiot_dns_event != NULL);
	rv = dns_gethostbyname(host, &ip, dns_host_found_cb, addr);

	switch(rv) {
	case ERR_OK:
		if(ip.type == IPADDR_TYPE_V4 || ip.type == IPADDR_TYPE_ANY) {
			addr->version = 4;
			addr->addr.ip4_addr.ip = ip.u_addr.ip4.addr;
		} else {
			addr->version = 6;
			memcpy(addr->addr.ip6_addr.ip, ip.u_addr.ip6.addr, sizeof(addr->addr.ip6_addr.ip));
		}

		rv = -EOK;
		break;

	case ERR_INPROGRESS:
		rv = lwiot_event_wait(lwiot_dns_event, FOREVER);
		break;

	default:
		return -EINVALID;
	}

	return rv;
}
