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
#include <lwiot/network/httpserver.h>
#include <lwiot/test.h>
#include <lwiot/network/sockettcpserver.h>

static volatile bool running = true;
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
		"<form action=\"/\" method=\"POST\" enctype=\"multipart/form-data\">"
		"<P>"
		"<input type=\"file\" name=\"cert\" id=\"fileToUpload\">"
		"<input type=\"submit\" value=\"Upload Image\" name=\"submit\">"
		"</P>"
		"</form>"
		"</body>"
		"</html>";

void returnFail(lwiot::HttpServer &server, const lwiot::String &msg)
{
	server.sendHeader("Connection", "close");
	server.sendHeader("Access-Control-Allow-Origin", "*");
	server.send(500, "text/plain", msg + "\r\n");
}

static void handle_root(lwiot::HttpServer &server)
{
	if(server.hasArg("cert")) {
		printf("Page submitted\n");
		//handleSubmit(server);
	} else {
		server.send(200, "text/html", INDEX_HTML);
	}
}

static void handle_upload(lwiot::HttpServer &server)
{
	lwiot::HTTPUpload &upload = server.upload();
	char *data;

	print_dbg("Http Upload!\n");

	switch(upload.status) {
	case lwiot::UPLOAD_FILE_START:
		print_dbg("Upload started: %s\n", upload.filename.c_str());
		break;

	case lwiot::UPLOAD_FILE_WRITE:
		data = new char[upload.currentSize+1];
		memcpy(data, upload.buf, upload.currentSize);
		data[upload.currentSize] = 0;
		print_dbg("Data: %s\n", data);
		delete[] data;
		break;

	case lwiot::UPLOAD_FILE_END:
		print_dbg("Upload ended!\n");
		running = false;
		break;

	default:
		print_dbg("Upload aborted!\n");
		running = false;
		break;
	}
}

static void test_httpserver()
{
	lwiot::SocketTcpServer *srv = new lwiot::SocketTcpServer(BIND_ADDR_ANY, 8080);
	lwiot::HttpServer server(srv);

	server.on("/", lwiot::HTTP_GET, handle_root);
	server.on("/", lwiot::HTTP_POST, [](lwiot::HttpServer &server) {
		server.send(200, "text/plain", "");
	}, handle_upload);

	assert(server.begin());
	print_dbg("HttpServer started!\n");

	while(running || server.hasClient()) {
		server.handleClient();
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
