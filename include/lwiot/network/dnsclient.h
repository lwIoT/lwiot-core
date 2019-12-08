/*
 * DNS client wrapper.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file dnsclient.h

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/network/udpclient.h>
#include <lwiot/network/stdnet.h>

#include <lwiot/kernel/lock.h>

namespace lwiot
{
	/**
	 * @brief DNS client.
	 */
	class DnsClient {
	public:
		/**
		 * @brief Construct DNS client.
		 */
		explicit DnsClient();

		/**
		 * @brief Lookup a domain name.
		 * @param domain Domain name to lookup.
		 * @return The IPAddress of \p domain or \p IP_ANY.
		 */
		IPAddress lookup(const stl::String& domain) const;

	private:
		mutable Lock _lock;
	};
}
