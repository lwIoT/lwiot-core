/*
 * ESP32 WiFi station implementation.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <lwiot.h>

#include <lwip/ip_addr.h>
#include <lwiot/log.h>
#include <lwiot/network/wifistation.h>
#include <lwiot/types.h>

#include <lwip/err.h>
#include <esp_err.h>
#include <esp_wifi.h>
#include <lwip/dns.h>

extern "C" {
	extern void esp32_wifi_subsys_init(void);
	extern void esp32_wifi_init_softap(const char *ssid, const char *pass, int max, uint8_t hidden, int channel);
	extern void esp32_wifi_init_station(const char *ssid, const char *pass);
}

extern "C" void esp32_wifi_station_event(system_event_t *event)
{
	auto& station = lwiot::WifiStation::instance();
	auto addr = lwiot::IPAddress((uint32_t)0);

	switch((int)event->event_id) {
		case SYSTEM_EVENT_STA_GOT_IP:
			addr = lwiot::IPAddress((uint32_t)event->event_info.got_ip.ip_info.ip.addr);
			station.setStatus(WL_CONNECTED);
			station.setAddress(addr);
			break;

		case SYSTEM_EVENT_STA_DISCONNECTED:
			station.setStatus(WL_DISCONNECTED);
			station.setAddress(addr);
			break;

		default:
			break;
	}
}

namespace lwiot
{
	WifiStation::WifiStation() : addr((uint32_t)0), ssid(""), password(""), _status(WL_IDLE_STATUS)
	{
		esp32_wifi_subsys_init();
	}

	void WifiStation::connectTo(const String& ssid)
	{
		esp32_wifi_init_station(ssid.c_str(), "");
	}

	void WifiStation::connectTo(const String& ssid, const String& pass)
	{
		esp32_wifi_init_station(ssid.c_str(), pass.c_str());
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
