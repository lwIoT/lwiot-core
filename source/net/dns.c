/*
 * DNS wrapper implementation.
 *
 * @author Michel Megens
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/error.h>
#include <lwiot/types.h>

#ifdef WIN32
#else
#include <errno.h>
#include <netdb.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#endif

#include "dns.h"

int hostname_to_ip(const char *host, uint32_t *addr)
{
	struct hostent *he;
	struct in_addr **addr_list;

	he = gethostbyname(host);

	if(he == NULL)
		return -EINVALID;

	addr_list = (struct in_addr**) he->h_addr_list;
	struct in_addr *inaddr = addr_list[0];

	if(inaddr == NULL)
		return -ENOTFOUND;

	*addr = inaddr->s_addr;
	return -EOK;
}
