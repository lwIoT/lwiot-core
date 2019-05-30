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
#include <lwiot/network/mqttclient.h>
#include <lwiot/network/sockettcpserver.h>
#include <lwiot/network/sockettcpclient.h>
#include <lwiot/kernel/thread.h>

class MqttClientTest : public lwiot::Functor {
	void run() override
	{
		lwiot::SocketTcpClient client("mail.sonatolabs.com", 1883);
		lwiot::MqttClient mqtt;

		mqtt.begin(client);
		mqtt.setCallback([](const lwiot::stl::String& topic, const lwiot::ByteBuffer& payload) {
			lwiot::stl::String msg(payload);
			print_dbg("Received MQTT message on topic: %s\n", topic.c_str());
			print_dbg("Received msg: %s\n", msg.c_str());
		});

		mqtt.connect("lwiot-test", "test", "test");
		mqtt.subscribe("test/subscribe");

		while(true) {
			print_dbg("Connected? :: %u :: State: %i\n", mqtt.connected(), mqtt.state());
			mqtt.publish("test/publish", "Test message");
			mqtt.loop();
			fflush(stdout);
			lwiot::Thread::sleep(1000);
		}
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
