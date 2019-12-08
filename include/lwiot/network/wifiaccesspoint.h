/*
 * WiFi station (mode) class definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file wifiaccesspoint.h

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <lwiot.h>

#include <lwiot/network/ipaddress.h>
#include <lwiot/log.h>

namespace lwiot
{
	/**
	 * @brief WiFi access point abstraction.
	 * @ingroup net
	 */
	class WifiAccessPoint {
	public:
		explicit WifiAccessPoint(); //!< Construct a new access point.
		virtual ~WifiAccessPoint() = default; //!< Destroy an access point object.

		WifiAccessPoint(const WifiAccessPoint&) = delete;
		void operator =(const WifiAccessPoint&) = delete;

		virtual void start() = 0; //!< Start the WiFi peripheral.

		/**
		 * @brief Start the access point.
		 * @param ssid WiFi SSID.
		 * @param pass WiFi password.
		 * @param chan Channel number.
		 * @param hidden Hidden flag.
		 * @param max Maximum number of clients.
		 */
		virtual void begin(const String& ssid, const String& pass, int chan, bool hidden, int max);

		/**
		 * @brief Configure the access point.
		 * @param local Local address.
		 * @param gw Gateway address.
		 * @param subnet Subnet.
		 */
		virtual void config(const IPAddress& local, const IPAddress& gw, const IPAddress& subnet);
		virtual operator bool() const = 0; //!< Bool conversion operator.
		virtual void stop() = 0; //!< Stop the access point.

		virtual String mac() const = 0; //!< Get the MAC address.

		/**
		 * @brief Get the hostname.
		 * @return The hostname.
		 */
		const String& hostname() const;

		/**
		 * @brief Set the hostname.
		 */
		void setHostname(const String& host);

		const String& ssid() const; //!< Get the SSID.
		const String& password() const; //!< Get the password.
		int channel() const; //!< Get the channel.
		bool hidden() const; //!< Get the hidden flag.
		int max() const; //!< Get the maximum number of alllowed clients.

	protected:
		IPAddress _local, _gw, _subnet;

	private:
		String _host;
		String _ssid;
		String _password;
		int _channel;
		bool _hidden;
		int _maxclients;
	};
}

