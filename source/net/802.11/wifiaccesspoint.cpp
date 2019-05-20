/*
 * WiFi accesspoint (mode) base class implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <lwiot.h>

#include <lwiot/log.h>

#include <lwiot/network/wifiaccesspoint.h>
#include <lwiot/network/ipaddress.h>

namespace lwiot
{
	WifiAccessPoint::WifiAccessPoint() : _local(0U), _gw(0U), _subnet(0U), _host("localhost")
	{
	}

	const String& WifiAccessPoint::hostname() const
	{
		return this->_host;
	}

	void WifiAccessPoint::setHostname(const lwiot::String &host)
	{
		this->_host = host;
	}

	const String& WifiAccessPoint::ssid() const
	{
		return this->_ssid;
	}

	const String& WifiAccessPoint::password() const
	{
		return this->_password;
	}

	int WifiAccessPoint::channel() const
	{
		return this->_channel;
	}

	bool WifiAccessPoint::hidden() const
	{
		return this->_hidden;
	}

	int WifiAccessPoint::max() const
	{
		return this->_maxclients;
	}

	void WifiAccessPoint::begin(const lwiot::String &ssid, const lwiot::String &pass, int chan, bool hidden, int max)
	{
		this->_ssid = ssid;
		this->_password = pass;
		this->_channel = chan;
		this->_hidden = hidden;
		this->_maxclients = max;
	}

	void WifiAccessPoint::config(const lwiot::IPAddress &local, const lwiot::IPAddress &gw, const lwiot::IPAddress &sn)
	{
		this->_local = local;
		this->_gw = gw;
		this->_subnet = sn;
	}
}
