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
	class WifiStation {
	public:
		static WifiStation& instance()
		{
			static WifiStation instance;
			return instance;
		}
		virtual ~WifiStation() = default;

		WifiStation(const WifiStation&) = delete;
		void operator =(const WifiStation&) = delete;

		void connectTo(const String& ssid);
		void connectTo(const String& ssid, const String& password);

		void setStatus(wireless_status_t status);
		wireless_status_t status() const;
		const IPAddress& address() const;
		void setAddress(const IPAddress& addr);

		operator bool () const;

	private:
		IPAddress addr;
		String ssid;
		String password;
		wireless_status_t _status;

		explicit WifiStation();
	};
}
