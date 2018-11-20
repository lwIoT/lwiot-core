/*
 * DNS resolver.
 *
 * @author: Michel Megens
 * @email:  dev@bietje.net
 */

#include <lwiot.h>
#include <stdlib.h>
#include <netdb.h>

#include <lwiot/types.h>
#include <lwiot/error.h>


#include <arpa/inet.h>
#include <sys/socket.h>

int lwiot_hostname_to_ip(const char *host, uint32_t *addr)
{
	struct hostent *he;
	struct in_addr **addr_list;

	he = gethostbyname(host);

	if(he == NULL)
		return -EINVALID;

	addr_list = (struct in_addr **) he->h_addr_list;
	struct in_addr *inaddr = addr_list[0];

	if(inaddr == NULL)
		return -ENOTFOUND;

	*addr = inaddr->s_addr;
	return -EOK;
}
