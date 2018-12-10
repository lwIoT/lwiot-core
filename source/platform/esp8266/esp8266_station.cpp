/*
 * ESP8266 WiFi station implementation.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <esp8266.h>

#include <lwiot/lwiot.h>
#include <lwiot/network/wifistation.h>

#include <espressif/esp_common.h>

namespace lwiot
{
	WifiStation::WifiStation() : addr((uint32_t)0), ssid(""), password(""), _status(WL_IDLE_STATUS)
	{
	}

	void WifiStation::connectTo(const String& ssid)
	{
		this->connectTo(ssid, "");
	}

	void WifiStation::connectTo(const String& ssid, const String& pass)
	{
		struct sdk_station_config wificonfig;

		auto mode = sdk_wifi_get_opmode();

		if(mode == SOFTAP_MODE)
			sdk_wifi_set_opmode(STATIONAP_MODE);
		else if(mode != STATIONAP_MODE)
			sdk_wifi_set_opmode(STATION_MODE);

		strcpy((char*)wificonfig.ssid, ssid.c_str());
		if(pass.length() == 0)
			wificonfig.password[0] = '\0';
		else
			strcpy((char*)wificonfig.password, pass.c_str());
		sdk_wifi_station_set_config(&wificonfig);

		if(sdk_wifi_station_connect()) {
			this->_status = WL_CONNECTED;
			this->password = pass;
			this->ssid = ssid;
		} else {
			this->_status = WL_CONNECT_FAILED;
		}
	}

	void WifiStation::setAddress(const IPAddress& addr)
	{
		this->addr = addr;
	}

	void WifiStation::setStatus(wireless_status_t status)
	{
		this->_status = status;
	}

	WifiStation::operator bool() const
	{
		return this->_status == WL_CONNECTED;
	}

	wireless_status_t WifiStation::status() const
	{
		return this->_status;
	}

	const IPAddress& WifiStation::address() const
	{
		return this->addr;
	}
}
