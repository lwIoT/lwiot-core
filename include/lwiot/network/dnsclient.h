/*
 * DNS client wrapper.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/network/udpclient.h>
#include <lwiot/network/stdnet.h>

#include <lwiot/kernel/lock.h>

namespace lwiot
{
	class DnsClient {
	public:
		explicit DnsClient();

		IPAddress lookup(const stl::String& domain) const;

	private:
		mutable Lock _lock;
	};
}
