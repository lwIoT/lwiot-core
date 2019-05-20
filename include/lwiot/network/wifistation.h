/*
 * WiFi station (mode) class definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/stl/string.h>
#include <lwiot/network/ipaddress.h>

namespace lwiot
{
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

	class WifiStation {
	public:
		explicit WifiStation();
		virtual ~WifiStation() = default;

		WifiStation(const WifiStation&) = delete;
		void operator =(const WifiStation&) = delete;

		virtual void connectTo(const String& ssid);
		virtual void connectTo(const String& ssid, const String& password);
		virtual void disconnect();

		virtual void setStatus(wireless_status_t status);
		wireless_status_t status() const;

		const IPAddress& address() const;
		virtual void setAddress(const IPAddress& addr);

		virtual operator bool () const = 0;

	protected:
		IPAddress _addr;
		wireless_status_t _status;

	private:
		String _ssid;
		String _password;

	};
}
