/*
 * WiFi station (mode) class definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <lwiot.h>

#include <lwiot/ipaddress.h>
#include <lwiot/log.h>


namespace lwiot
{
	class WifiAccessPoint {
	public:
		static WifiAccessPoint& instance()
		{
			static WifiAccessPoint instance;
			return instance;
		}

		WifiAccessPoint(const WifiAccessPoint&) = delete;
		void operator =(const WifiAccessPoint&) = delete;

		void start();

		void begin(const String& ssid, const String& pass = "", int chan = 1, bool hidden = false, int max = 4);
		void config(const IPAddress& local, const IPAddress& gw, const IPAddress& sn);
		operator bool() const;
		void end();

		String mac() const;
		const String& hostname() const;
		void setHostname(const String& host);

	private:
		IPAddress _local, _gw, _subnet;
		String _host;

		explicit WifiAccessPoint();
		virtual ~WifiAccessPoint() = default;
	};
}

