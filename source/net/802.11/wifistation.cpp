/*
 * WiFi station mode implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/stl/string.h>

#include <lwiot/network/ipaddress.h>
#include <lwiot/network/wifistation.h>

namespace lwiot
{
	WifiStation::WifiStation() : _addr(0U), _ssid(""), _password(""), _status(WL_DISCONNECTED)
	{
	}

	void WifiStation::connectTo(const lwiot::String &ssid)
	{
		this->_ssid = ssid;
	}

	void WifiStation::connectTo(const lwiot::String &ssid, const lwiot::String &password)
	{
		this->_ssid = ssid;
		this->_password = password;
	}

	void WifiStation::setStatus(lwiot::wireless_status_t status)
	{
		this->_status = status;
	}

	wireless_status_t WifiStation::status() const
	{
		return this->_status;
	}

	const IPAddress& WifiStation::address() const
	{
		return this->_addr;
	}

	void WifiStation::setAddress(const lwiot::IPAddress &addr)
	{
		this->_addr = addr;
	}
}
