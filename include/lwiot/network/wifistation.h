/*
 * WiFi station (mode) class definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file wifistation.h

#pragma once

#include <lwiot/stl/string.h>
#include <lwiot/network/ipaddress.h>

namespace lwiot
{
	/**
	 * @brief WiFi status codes.
	 */
	typedef enum {
		WL_NO_SHIELD        = 255,
		WL_IDLE_STATUS      = 0,
		WL_NO_SSID_AVAIL    = 1,
		WL_SCAN_COMPLETED   = 2,
		WL_CONNECTED        = 3,
		WL_CONNECT_FAILED   = 4,
		WL_CONNECTION_LOST  = 5,
		WL_DISCONNECTED     = 6
	} wireless_status_t;

	/**
	 * @brief WiFi station.
	 */
	class WifiStation {
	public:
		explicit WifiStation(); //!< Construct a new WiFi station object.
		virtual ~WifiStation() = default;

		WifiStation(const WifiStation&) = delete;
		void operator =(const WifiStation&) = delete;

		/**
		 * @brief Connect to an access point.
		 * @param ssid SSID to connect to.
		 */
		virtual void connectTo(const String& ssid);

		/**
		 * @brief Connect to an access point.
		 * @param ssid SSID to connect to.
		 * @param password The password of \p ssid.
		 */
		virtual void connectTo(const String& ssid, const String& password);
		virtual void disconnect(); //!< Disconnect from an access point.

		/**
		 * @brief Set the station status.
		 * @param status Status code to set.
		 */
		virtual void setStatus(wireless_status_t status);
		wireless_status_t status() const; //!< Get the status code.

		const IPAddress& address() const; //!< Get the station address.

		/**
		 * @brief Set the station address.
		 * @param addr The address to set.
		 */
		virtual void setAddress(const IPAddress& addr);

		virtual operator bool () const = 0; //!< Bool conversion operator.

	protected:
		IPAddress _addr; //!< Station address.
		wireless_status_t _status; //!< WiFi status code.

	private:
		String _ssid;
		String _password;

	};
}
