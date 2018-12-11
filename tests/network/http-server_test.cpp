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
#include <lwiot/network/sockettcpserver.h>

static const char INDEX_HTML[] =
		"<!DOCTYPE HTML>"
		"<html>"
		"<head>"
		"<meta name = \"viewport\" content = \"width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0\">"
		"<title>ESP8266 Web Form Demo</title>"
		"<style>"
		"\"body { background-color: #808080; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }\""
		"</style>"
		"</head>"
		"<body>"
		"<h1>ESP8266 Web Form Demo</h1>"
		"<FORM action=\"/\" method=\"post\">"
		"<P>"
		"LED<br>"
		"<INPUT type=\"radio\" name=\"LED\" value=\"1\">On<BR>"
		"<INPUT type=\"radio\" name=\"LED\" value=\"0\">Off<BR>"
		"<INPUT type=\"submit\" value=\"Send\"> <INPUT type=\"reset\">"
		"</P>"
		"</FORM>"
		"</body>"
		"</html>";

void returnFail(lwiot::HttpServer& server, const lwiot::String& msg)
{
	server.sendHeader("Connection", "close");
	server.sendHeader("Access-Control-Allow-Origin", "*");
	server.send(500, "text/plain", msg + "\r\n");
}

static void handleSubmit(lwiot::HttpServer& server)
{
	lwiot::String LEDvalue;

	if (!server.hasArg("LED")) return returnFail(server, "BAD ARGS");
	LEDvalue = server.arg("LED");
	if (LEDvalue == "1") {
		server.send(200, "text/html", INDEX_HTML);
	}
	else if (LEDvalue == "0") {
		server.send(200, "text/html", INDEX_HTML);
	}
	else {
		returnFail(server,"Bad LED value");
	}
}

static void handle_root(lwiot::HttpServer& server)
{
	if(server.hasArg("LED")) {
		printf("Page submitted");
		handleSubmit(server);
	} else {
		server.send(200, "text/html", INDEX_HTML);
	}
}

static void test_httpserver()
{
	lwiot::SocketTcpServer* srv = new lwiot::SocketTcpServer(BIND_ADDR_ANY, 8080);
	lwiot::HttpServer server(srv);
	bool running = true;

	server.on("/", handle_root);
	assert(server.begin());
	print_dbg("HttpServer started!\n");

	while(running) {
		server.handleClient();
		running = server.hasClient();
	}
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
