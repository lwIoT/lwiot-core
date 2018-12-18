/*
 * HTTP server.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/log.h>
#include <lwiot/string.h>
#include <lwiot/stream.h>
#include <lwiot/network/requesthandler.h>
#include <lwiot/function.h>

#include <lwiot/network/ipaddress.h>
#include <lwiot/network/tcpserver.h>
#include <lwiot/network/tcpclient.h>
#include <lwiot/uniquepointer.h>

namespace lwiot
{
	class HttpServer {
	public:
		explicit HttpServer(TcpServer* server);
		virtual ~HttpServer();

		virtual bool begin();

		virtual void handleClient();
		virtual void close();

		void stop();

		void requestAuthentication(HTTPAuthMethod mode = BASIC_AUTH, const char *realm = nullptr,
		                           const String &authFailMsg = String(""));

		using THandlerFunction = Function<void(*)(HttpServer& server)>;

		void on(const String &uri, THandlerFunction handler);
		void on(const String &uri, HTTPMethod method, THandlerFunction fn);
		void on(const String &uri, HTTPMethod method, THandlerFunction fn, THandlerFunction ufn);

		void addHandler(RequestHandler *handler);

		void onNotFound(THandlerFunction fn);  //called when handler is not assigned
		void onFileUpload(THandlerFunction fn); //handle file uploads

		String uri()
		{
			return _currentUri;
		}

		HTTPMethod method()
		{
			return _currentMethod;
		}

		HTTPUpload &upload()
		{
			return *_currentUpload;
		}

		bool hasClient() const;
		String arg(String name);        // get request argument value by name
		String arg(int i);              // get request argument value by number
		String argName(int i);          // get request argument name by number
		int args();                     // get arguments count
		bool hasArg(String name);       // check if argument exists
		void collectHeaders(const char *headerKeys[], size_t headerKeysCount); // set the request headers to collect
		String header(String name);      // get request header value by name
		String header(int i);              // get request header value by number
		String headerName(int i);          // get request header name by number
		int headers();                     // get header count
		bool hasHeader(String name);       // check if header exists

		String hostHeader();            // get request host header if available or empty String if not

		void send(int code, const char *content_type = nullptr, const String &content = String(""));
		void send(int code, char *content_type, const String &content);
		void send(int code, const String &content_type, const String &content);


		void setContentLength(size_t contentLength);
		void sendHeader(const String &name, const String &value, bool first = false);
		void sendContent(const String &content);

		static String urlDecode(const String &text);

		template<typename T>
		size_t streamFile(T &file, const String &contentType)
		{
			_streamFileCore(file.size(), file.name(), contentType);
			return _currentClient->write(file);
		}

	protected:
		virtual size_t _currentClientWrite(const char *b, size_t l)
		{
			_currentClient->write(b, l);
			return l;
		}

		void _addRequestHandler(RequestHandler *handler);
		void _handleRequest();
		void _finalizeResponse();
		bool _parseRequest(TcpClient &client);
		void _parseArguments(String data);

		static String _responseCodeToString(int code);

		bool _parseForm(TcpClient &client, String boundary, uint32_t len);
		bool _parseFormUploadAborted();
		void _uploadWriteByte(uint8_t b);
		uint8_t _uploadReadByte(TcpClient &client);

		void _prepareHeader(String &response, int code, const char *content_type, size_t contentLength);
		bool _collectHeader(const char *headerName, const char *headerValue);

		void _streamFileCore(size_t fileSize, const String &fileName, const String &contentType);
		String _getRandomHexString();
		String _extractParam(String &authReq, const String &param, char delimit = '"');

		struct RequestArgument {
			String key;
			String value;
		};

		UniquePointer<TcpServer> _server;
		UniquePointer<TcpClient> _currentClient;

		HTTPMethod _currentMethod;
		String _currentUri;
		uint8_t _currentVersion;
		HTTPClientStatus _currentStatus;
		unsigned long _statusChange;

		RequestHandler *_currentHandler;
		RequestHandler *_firstHandler;
		RequestHandler *_lastHandler;
		THandlerFunction _notFoundHandler;
		THandlerFunction _fileUploadHandler;

		int _currentArgCount;
		RequestArgument *_currentArgs;
		lwiot::UniquePointer<HTTPUpload> _currentUpload;

		int _headerKeysCount;
		RequestArgument *_currentHeaders;
		size_t _contentLength;
		String _responseHeaders;

		String _hostHeader;
		bool _chunked;

		String _snonce;
		String _sopaque;
		String _srealm;

	};
}
