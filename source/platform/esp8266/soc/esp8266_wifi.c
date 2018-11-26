/*
 * Low level ESP8266 WiFi implementation.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <lwiot.h>

#include <esp_system.h>
#include <esp_wifi.h>
#include <esp_event_loop.h>
#include <nvs_flash.h>

#include <lwiot/lwiot.h>
#include <lwiot/log.h>

#include <lwip/err.h>
#include <lwip/sys.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>

static EventGroupHandle_t wifi_events;
static const int WIFI_CONNECTED = BIT0;
static bool initialised = false;

extern void esp8266_wifi_station_event(system_event_t *event);
extern void esp8266_wifi_ap_event(system_event_t *event);

static esp_err_t wifi_event_handler(void *ctx, system_event_t *event)
{
	switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        break;

    case SYSTEM_EVENT_STA_GOT_IP:
        print_dbg("[WIFI]: got ip: %s",
                 ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
        xEventGroupSetBits(wifi_events, WIFI_CONNECTED);
		esp8266_wifi_station_event(event);
        break;

    case SYSTEM_EVENT_AP_STACONNECTED:
    case SYSTEM_EVENT_AP_STADISCONNECTED:
		esp8266_wifi_ap_event(event);
        break;

    case SYSTEM_EVENT_STA_DISCONNECTED:
		esp_wifi_connect();
        xEventGroupClearBits(wifi_events, WIFI_CONNECTED);
		esp8266_wifi_station_event(event);
        break;

    default:
		break;
	}
	return ESP_OK;
}

void esp8266_wifi_subsys_init(void)
{
	esp_err_t ret;
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

	if(initialised)
		return;

	ret = nvs_flash_init();
	if(ret == ESP_ERR_NVS_NO_FREE_PAGES) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}

	ESP_ERROR_CHECK(ret);
	tcpip_adapter_init();

	wifi_events = xEventGroupCreate();
	esp_event_loop_init(wifi_event_handler, NULL);
	esp_wifi_init(&cfg);
	initialised = true;
}

void esp8266_wifi_init_softap(const char *ssid, const char *pass, int max, uint8_t hidden, int channel)
{
	wifi_config_t config = {
		.ap = {
			.ssid_len = strlen(ssid),
			.max_connection = max,
			.authmode = WIFI_AUTH_WPA_WPA2_PSK, 
			.ssid_hidden = hidden,
			.channel = channel
		}
	};

	if(pass == NULL || strlen(pass) == 0)
		config.ap.authmode = WIFI_AUTH_OPEN;

	strcpy(config.ap.ssid, ssid);
	strcpy(config.ap.password, pass);

	esp_wifi_stop();
	esp_wifi_set_mode(WIFI_MODE_AP);
	esp_wifi_set_config(ESP_IF_WIFI_AP, &config);
	esp_wifi_start();
}

void esp8266_wifi_init_station(const char *ssid, const char *pass)
{
	wifi_config_t config = {
		.sta = {
			.channel = 1
		}
	};

	strcpy(config.sta.ssid, ssid);
	strcpy(config.sta.password, pass);

	esp_wifi_stop();
	esp_wifi_set_mode(WIFI_MODE_STA);
	esp_wifi_set_config(ESP_IF_WIFI_STA, &config);
	esp_wifi_start();
}