/*
 * Generic TCP server unit test.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>
#include <assert.h>

#include <lwiot/log.h>
#include <lwiot/httpserver.h>
#include <lwiot/test.h>

static void handle_root(lwiot::HttpServer& server)
{
	char temp[400];
	int sec = lwiot_tick_ms() / 1000;
	int min = sec / 60;
	int hr = min / 60;

	snprintf(temp, 400,
	         "<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>ESP8266 Demo</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Hello from ESP8266!</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <img src=\"/test.svg\" />\
  </body>\
</html>",

	         hr, min % 60, sec % 60
	);

	server.send(200, "text/html", temp);
}

static void test_httpserver(void)
{
	lwiot::IPAddress addr(127,0,0,1);
	lwiot::HttpServer server(addr, 8000);

	server.on("/", handle_root);
	assert(server.begin());
	print_dbg("HttpServer started!\n");
	while(true)
		server.handleClient();
	server.close();
}

int main(int argc, char **argv)
{
	lwiot_init();

	print_dbg("Testing HttpServer implementation!\n");
	test_httpserver();
	lwiot_destroy();
	wait_close();

	return -EXIT_SUCCESS;
}
