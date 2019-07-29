/*
 * MQTT client test.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <lwiot.h>
#include <stdio.h>
#include <stdlib.h>

#include <lwiot/util/application.h>
#include <lwiot/functor.h>
#include <lwiot/log.h>
#include <lwiot/test.h>

#include <lwiot/network/ipaddress.h>
#include <lwiot/network/asyncmqttclient.h>
#include <lwiot/network/sockettcpserver.h>
#include <lwiot/network/sockettcpclient.h>
#include <lwiot/network/dnsclient.h>

#include <lwiot/kernel/thread.h>

class MqttClientTest : public lwiot::Functor {
	void run() override
	{
		lwiot::DnsClient dns;
		lwiot::IPAddress host = lwiot::stl::move(dns.lookup("mail.sonatolabs.com"));

		lwiot::SocketTcpClient client(host, 1883);
		lwiot::AsyncMqttClient mqtt;
		int a = 0,
		    b = 0;

		mqtt.start(client);

		mqtt.connect("lwiot-test", "test", "test");
		mqtt.subscribe("test/subscribe/1", [&a](const lwiot::ByteBuffer& payload) {
			lwiot::stl::String str(payload);

			a++;
			print_dbg("Data (subscribe/1): %s\n", str.c_str());
		}, lwiot::MqttClient::QOS0);

		mqtt.subscribe("test/subscribe/2", [&b](const lwiot::ByteBuffer& payload) {
			lwiot::stl::String str(payload);

			b++;
			print_dbg("Data (subscribe/2): %s\n", str.c_str());
		}, lwiot::MqttClient::QOS0);

		for(int idx = 0; idx < 5; idx++) {
			print_dbg("Connected? :: %u :: State: %i\n", mqtt.connected(), mqtt.state());

			mqtt.publish("test/subscribe/1", "Test message for s1", false);
			mqtt.publish("test/subscribe/2", "Test message for s2", false);

			fflush(stdout);
			lwiot::Thread::sleep(1000);
		}

		mqtt.stop();

		assert(a == 5);
		assert(b == 5);
	}
};

int main(int argc, char **argv)
{
	MqttClientTest test;
	lwiot::Application testapp(test);

	testapp.start();
	wait_close();

	return -EXIT_SUCCESS;
}
