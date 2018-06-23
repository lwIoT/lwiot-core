/*
 * @brief ESP32 software access point implementation.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <esp_err.h>
#include <esp_wifi.h>
#include <lwiot.h>

#include <lwiot/log.h>
#include <lwiot/wifiaccesspoint.h>
#include <lwiot/types.h>

#include <lwip/err.h>
#include <lwip/dns.h>

#include <apps/dhcpserver.h>
#include <apps/dhcpserver_options.h>

extern "C" {
	extern void esp32_wifi_subsys_init(void);
	extern void esp32_wifi_init_softap(const char *ssid, const char *pass, int max, uint8_t hidden, int channel);
	extern void esp32_wifi_init_station(const char *ssid, const char *pass);
}

extern "C" void esp32_wifi_ap_event(system_event_t *event)
{
	switch(event->event_id) {
	case SYSTEM_EVENT_AP_STACONNECTED:
		print_dbg("IPv4 client connected!\n");
		break;

	case SYSTEM_EVENT_AP_STADISCONNECTED:
		print_dbg("WiFi client disconnected\n");
		break;

	default:
		break;
	}
}

namespace lwiot
{
	WifiAccessPoint::WifiAccessPoint() : initialised(false)
	{
	}

	void WifiAccessPoint::begin(const String& ssid, const String& pass, int chan, bool hidden, int max)
	{
		if(!initialised)
			esp32_wifi_subsys_init();

		esp32_wifi_init_softap(ssid.c_str(), pass.c_str(), max, hidden, chan);
	}

	void WifiAccessPoint::config(const IPAddress& local, const IPAddress& gw, const IPAddress& sn)
	{
		tcpip_adapter_ip_info_t info;

		esp_wifi_start();
		info.ip.addr = static_cast<uint32_t>(local);
		info.gw.addr = static_cast<uint32_t>(gw);
		info.netmask.addr = static_cast<uint32_t>(sn);
		tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_AP);

		if(tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_AP, &info) == ESP_OK) {
			dhcps_lease_t lease;
			lease.enable = true;
			lease.start_ip.addr = static_cast<uint32_t>(local) + (1 << 24);
			lease.end_ip.addr = static_cast<uint32_t>(local) + (11 << 24);

			tcpip_adapter_dhcps_option(
				(tcpip_adapter_option_mode_t)TCPIP_ADAPTER_OP_SET,
				(tcpip_adapter_option_id_t)REQUESTED_IP_ADDRESS,
				(void*)&lease, sizeof(dhcps_lease_t)
			);

			tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_AP);
		} else {
			tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_AP);
		}

		this->_local = local;
		this->_gw = gw;
		this->_subnet = sn;
	}

	void end()
	{
		wifi_mode_t mode;

		/* TODO: NULLIFY config */

		esp_wifi_get_mode(&mode);
		if(mode == WIFI_MODE_APSTA)
			esp_wifi_set_mode(WIFI_MODE_STA);
		else
			esp_wifi_set_mode(WIFI_MODE_NULL);
	}
}
