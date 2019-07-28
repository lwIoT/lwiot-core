/*
 * DNS client wrapper.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/network/udpclient.h>
#include <lwiot/network/dnsclient.h>

#include <lwiot/kernel/lock.h>
#include <lwiot/kernel/uniquelock.h>

namespace lwiot
{
	DnsClient::DnsClient() : _lock(false)
	{
	}

	IPAddress DnsClient::lookup(const lwiot::String &domain) const
	{
		if(domain.length() <= 0)
			return IPAddress();

		UniqueLock<Lock> lock(this->_lock);
		auto raw = domain.c_str();
		remote_addr_t remote;

		if(dns_resolve_host(raw, &remote) != -EOK)
			return IPAddress(0,0,0,0);

		return stl::move(IPAddress(remote));
	}
}
