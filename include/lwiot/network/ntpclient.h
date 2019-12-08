/*
 * NTP client implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file ntpclient.h

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/network/udpclient.h>
#include <lwiot/network/ipaddress.h>

#include <lwiot/stl/referencewrapper.h>

namespace lwiot
{
	/**
	 * @brief NTP client.
	 * @ingroup net
	 */
	class NtpClient {
	public:
		explicit NtpClient(); //!< Construct a NTP client.

		/**
		 * @brief Construct a NTP client.
		 * @param client UDP client.
		 */
		explicit NtpClient(UdpClient& client);

		void begin(); //!< Start the NTP client.
		void begin(UdpClient& client); //!< Start the NTP client.
		bool update(); //!< Update the time.
		time_t time() const; //!< Get the current time.

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
