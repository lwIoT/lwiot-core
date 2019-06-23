/*
 * NTP client implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/network/udpclient.h>
#include <lwiot/network/ipaddress.h>

#include <lwiot/stl/referencewrapper.h>

namespace lwiot
{
	class NtpClient {
	public:
		explicit NtpClient();
		explicit NtpClient(UdpClient& client);

		void begin();
		void begin(UdpClient& client);
		bool update();
		time_t time() const;

	private:
		mutable stl::ReferenceWrapper<UdpClient> _client;

		time_t _current_epoch;
		time_t _last_update;

		constexpr static int NTP_PACKET_SIZE = 48;
		constexpr static int NTP_DEFAULT_PORT = 1337;
		constexpr static unsigned long SEVENTY_YEARS = 2208988800UL;

		/* Methods */
		void sendRequest() const;
	};
}
