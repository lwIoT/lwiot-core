#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <esp_attr.h>
#include <lwiot.h>

#include <lwiot/kernel/thread.h>
#include <lwiot/stl/string.h>
#include <lwiot/io/uart.h>
#include <lwiot/stl/string.h>
#include <lwiot/io/watchdog.h>

#include <lwiot/network/xbee/xbeeaddress.h>
#include <lwiot/network/xbee/xbeeresponse.h>
#include <lwiot/network/xbee/xbeerequest.h>
#include <lwiot/network/xbee/xbee.h>

#include <lwiot/esp32/esp32uart.h>

class MainThread : public lwiot::Thread {
public:
	explicit MainThread(const char *arg) : Thread("main-thread", (void *) arg)
	{
	}

protected:
	void run()
	{
		lwiot::esp32::Uart uart(2, 9600);
		lwiot::XBee xbee;
		lwiot::Rx16Response rx16 = lwiot::Rx16Response();
		lwiot::Rx64Response rx64 = lwiot::Rx64Response();

		printf("Main thread started!\n");
		xbee.begin(uart);
		wdt.disable();

		while(true) {
			xbee.readPacket();

			if (xbee.getResponse().isAvailable()) {
				print_dbg("Packet received!\n");
				if (xbee.getResponse().getApiId() == RX_16_RESPONSE || xbee.getResponse().getApiId() == RX_64_RESPONSE) {
					uint8_t *data;

					if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
						xbee.getResponse().getRx16Response(rx16);
						data = rx16.getData();
					} else {
						xbee.getResponse().getRx64Response(rx64);
						data = rx64.getData();
					}

					print_dbg("Received: %c%c%c\n", data[0], data[1], data[2]);
				} else {
					print_dbg("Unexpected response..\n");
				}
			} else if (xbee.getResponse().isError()) {
				print_dbg("Xbee error: %u\n", xbee.getResponse().getErrorCode());
			}
		}
	}
};

static MainThread *mt;
const char *arg = "Hello, World! [FROM main-thread]";

extern "C" void main_start(void)
{
	printf("Creating main thread..");
	mt = new MainThread(arg);
	printf("[DONE]\n");
	mt->start();
}

