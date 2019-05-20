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

#include <lwiot/network/ipaddress.h>
#include <lwiot/log.h>

namespace lwiot
{
	class WifiAccessPoint {
	public:
		explicit WifiAccessPoint();
		virtual ~WifiAccessPoint() = default;

		WifiAccessPoint(const WifiAccessPoint&) = delete;
		void operator =(const WifiAccessPoint&) = delete;

		virtual void start() = 0;

		virtual void begin(const String& ssid, const String& pass, int chan, bool hidden, int max);
		virtual void config(const IPAddress& local, const IPAddress& gw, const IPAddress& subnet);
		virtual operator bool() const = 0;
		virtual void stop() = 0;

		virtual String mac() const = 0;
		const String& hostname() const;
		void setHostname(const String& host);

		const String& ssid() const;
		const String& password() const;
		int channel() const;
		bool hidden() const;
		int max() const;

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

