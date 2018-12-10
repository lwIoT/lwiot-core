/*
 * @brief ESP8266 software access point implementation.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <lwiot.h>

#include <lwiot/log.h>
#include <lwiot/network/wifiaccesspoint.h>
#include <lwiot/types.h>

#include <lwip/err.h>
#include <lwip/dns.h>

#include <espressif/esp_common.h>

namespace lwiot
{
	WifiAccessPoint::WifiAccessPoint()
	{
	}

	void WifiAccessPoint::start()
	{
	}

	void WifiAccessPoint::begin(const String& ssid, const String& pass, int chan, bool hidden, int max)
	{
		auto mode = sdk_wifi_get_opmode();
		struct sdk_softap_config config;

		if(this->_local == (uint32_t)0)
			return;

		if(mode == STATION_MODE)
			sdk_wifi_set_opmode(STATIONAP_MODE);
		else
			sdk_wifi_set_opmode(SOFTAP_MODE);

		strcpy((char*)config.ssid, ssid.c_str());
		strcpy((char*)config.password, pass.c_str());
		config.channel = chan;
		config.ssid_hidden = hidden;
		config.max_connection = max;
		config.ssid_len = ssid.length();
		config.authmode = AUTH_WPA_WPA2_PSK;

		if(pass.length() == 0)
			config.authmode = AUTH_OPEN;
		
		sdk_wifi_softap_set_config(&config);
	}

	void WifiAccessPoint::config(const IPAddress& local, const IPAddress& gw, const IPAddress& sn)
	{
		struct ip_info info;

		this->_local = local;
		this->_gw = gw;
		this->_subnet = sn;

		info.ip.addr = this->_local;
		info.gw.addr = this->_gw;
		info.netmask.addr = this->_subnet;
		sdk_wifi_set_ip_info(SOFTAP_IF, &info);
	}

	void WifiAccessPoint::end()
	{
		auto mode = sdk_wifi_get_opmode();

		if(mode == STATIONAP_MODE) {
			sdk_wifi_set_opmode(STATION_MODE);
		} else if(mode == SOFTAP_MODE) {
			sdk_wifi_set_opmode(NULL_MODE);
		}
	}

	WifiAccessPoint::operator bool() const
	{
		return this->_local != static_cast<uint32_t>(0);
	}

	const String& WifiAccessPoint::hostname() const
	{
		return this->_host;
	}

	void WifiAccessPoint::setHostname(const lwiot::String &host)
	{
		this->_host = host;
	}
}
