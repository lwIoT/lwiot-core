/*
 * HTTP server.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/log.h>
#include <lwiot/stl/string.h>
#include <lwiot/stream.h>
#include <lwiot/network/requesthandler.h>
#include <lwiot/function.h>
#include <lwiot/network/httpserver.h>
#include <lwiot/kernel/thread.h>
#include <lwiot/network/base64.h>

#include <lwiot/network/ipaddress.h>
#include <lwiot/network/tcpserver.h>
#include <lwiot/network/tcpclient.h>
#include <lwiot/stl/move.h>
#include <lwiot/bytebuffer.h>

#include "mimetable.h"
#include "requesthandlerimpl.h"

static const char Content_Type[] = "Content-Type";
static const char filename[] = "filename";

static const char AUTHORIZATION_HEADER[] = "Authorization";
static const char WWW_Authenticate[] = "WWW-Authenticate";
static const char Content_Length[] = "Content-Length";

#define F(__x__) __x__
#define FPSTR(__x__) __x__

namespace lwiot
{
	HttpServer::HttpServer(TcpServer* server)
			: _server(server), _currentMethod(HTTP_ANY), _currentVersion(0), _currentStatus(HC_NONE),
			  _statusChange(0), _currentHandler(nullptr), _firstHandler(nullptr), _lastHandler(nullptr),
			  _currentArgCount(0), _currentArgs(nullptr), _headerKeysCount(0), _currentHeaders(nullptr),
			  _contentLength(0), _chunked(false)
	{
	}

	HttpServer::~HttpServer()
	{
		_server->close();

		delete[]_currentHeaders;
		delete[] _currentArgs;

		RequestHandler *handler = _firstHandler;

		while(handler) {
			RequestHandler *next = handler->next();
			delete handler;
			handler = next;
		}
	}

	bool HttpServer::begin()
	{
		this->_server->connect();
		return this->_server->bind();
	}

	String HttpServer::_extractParam(String &authReq, const String &param, char delimit)
	{
		int _begin = authReq.indexOf(param);
		if(_begin == -1)
			return "";
		return authReq.substring(_begin + param.length(), authReq.indexOf(delimit, _begin + param.length()));
	}

	String HttpServer::_getRandomHexString()
	{
		char buffer[33];  // buffer to hold 32 Hex Digit + /0
		int i;
		int r = rand();
		for(i = 0; i < 4; i++) {
			sprintf(buffer + (i * 8), "%08x", r);
		}
		return String(buffer);
	}

	void HttpServer::requestAuthentication(HTTPAuthMethod mode, const char *realm, const String &authFailMsg)
	{
		if(realm == nullptr) {
			_srealm = String(F("Login Required"));
		} else {
			_srealm = String(realm);
		}

		if(mode == BASIC_AUTH) {
			sendHeader(String(FPSTR(WWW_Authenticate)), String(F("Basic realm=\"")) + _srealm + String(F("\"")));
		} else {
			_snonce = _getRandomHexString();
			_sopaque = _getRandomHexString();
			sendHeader(String(FPSTR(WWW_Authenticate)),
			           String(F("Digest realm=\"")) + _srealm + String(F("\", qop=\"auth\", nonce=\"")) + _snonce +
			           String(F("\", opaque=\"")) + _sopaque + String(F("\"")));
		}
		using namespace mime;
		send(401, String(FPSTR(mimeTable[html].mimeType)), authFailMsg);
	}

	void HttpServer::on(const String &uri, HttpServer::THandlerFunction handler)
	{
		on(uri, HTTP_ANY, handler);
	}

	void HttpServer::on(const String &uri, HTTPMethod method, HttpServer::THandlerFunction fn)
	{
		this->on(uri, method, fn, _fileUploadHandler);
	}

	void HttpServer::on(const String &uri, HTTPMethod method, HttpServer::THandlerFunction fn,
	                    HttpServer::THandlerFunction ufn)
	{
		_addRequestHandler(new FunctionRequestHandler(fn, ufn, uri, method));
	}

	void HttpServer::addHandler(RequestHandler *handler)
	{
		_addRequestHandler(handler);
	}

	void HttpServer::_addRequestHandler(RequestHandler *handler)
	{
		if(!_lastHandler) {
			_firstHandler = handler;
			_lastHandler = handler;
		} else {
			_lastHandler->next(handler);
			_lastHandler = handler;
		}
	}

	void HttpServer::handleClient()
	{
		if(_currentStatus == HC_NONE) {
			this->_currentClient.reset();
			this->_currentClient = stl::move(this->_server->accept());

			this->_currentClient->setTimeout(5);

			if(this->_currentClient.get() == nullptr) {
				return;
			}

			_currentStatus = HC_WAIT_READ;
			_statusChange = lwiot_tick_ms();
		}

		bool keepCurrentClient = false;

		if(_currentClient->connected()) {
			switch(_currentStatus) {
			case HC_NONE:
				break;

			case HC_WAIT_READ:
				if(this->_currentClient->available()) {
					if(_parseRequest(*_currentClient)) {
						//_currentClient->setTimeout(HTTP_MAX_SEND_WAIT);
						_contentLength = CONTENT_LENGTH_NOT_SET;
						_handleRequest();

						if(_currentClient->connected()) {
							_currentStatus = HC_WAIT_CLOSE;
							_statusChange = lwiot_tick_ms();
							keepCurrentClient = true;
						}
					}
				} else {
					if(lwiot_tick_ms() - _statusChange <= HTTP_MAX_DATA_WAIT) {
						keepCurrentClient = true;
					}
				}
				break;
			case HC_WAIT_CLOSE:
				if(lwiot_tick_ms() - _statusChange <= HTTP_MAX_CLOSE_WAIT) {
					keepCurrentClient = true;
				}
			}
		}

		if(!keepCurrentClient) {
			this->_currentClient->close();
			_currentStatus = HC_NONE;
			_currentUpload.reset();
		}

		/*if(callYield) {
			Thread::yield();
		}*/
	}

	void HttpServer::close()
	{
		_server->close();
		_currentStatus = HC_NONE;
		if(!_headerKeysCount)
			collectHeaders(nullptr, 0);
	}

	bool HttpServer::hasClient() const
	{
		if(!this->_currentClient)
			return false;

		return this->_currentStatus != HC_NONE;
	}

	void HttpServer::stop()
	{
		close();
	}

	void HttpServer::sendHeader(const String &name, const String &value, bool first)
	{
		String headerLine = name;
		headerLine += F(": ");
		headerLine += value;
		headerLine += "\r\n";

		if(first) {
			_responseHeaders = headerLine + _responseHeaders;
		} else {
			_responseHeaders += headerLine;
		}
	}

	void HttpServer::setContentLength(size_t contentLength)
	{
		_contentLength = contentLength;
	}

	void HttpServer::_prepareHeader(String &response, int code, const char *content_type, size_t contentLength)
	{
		response = String(F("HTTP/1.")) + String(_currentVersion) + ' ';
		response += String(code);
		response += ' ';
		response += _responseCodeToString(code);
		response += "\r\n";

		using namespace mime;
		if(!content_type)
			content_type = mimeTable[html].mimeType;

		sendHeader(String(F("Content-Type")), String(FPSTR(content_type)), true);
		if(_contentLength == CONTENT_LENGTH_NOT_SET) {
			sendHeader(String(FPSTR(Content_Length)), String(contentLength));
		} else if(_contentLength != CONTENT_LENGTH_UNKNOWN) {
			sendHeader(String(FPSTR(Content_Length)), String(_contentLength));
		} else if(_contentLength == CONTENT_LENGTH_UNKNOWN && _currentVersion) { //HTTP/1.1 or above client
			_chunked = true;
			sendHeader(String(F("Accept-Ranges")), String(F("none")));
			sendHeader(String(F("Transfer-Encoding")), String(F("chunked")));
		}
		sendHeader(String(F("Connection")), String(F("close")));

		response += _responseHeaders;
		response += "\r\n";
		_responseHeaders = "";
	}

	void HttpServer::send(int code, const char *content_type, const String &content)
	{
		String header;
		// Can we asume the following?
		//if(code == 200 && content.length() == 0 && _contentLength == CONTENT_LENGTH_NOT_SET)
		//  _contentLength = CONTENT_LENGTH_UNKNOWN;
		_prepareHeader(header, code, content_type, content.length());
		_currentClientWrite(header.c_str(), header.length());
		if(content.length())
			sendContent(content);
	}

	void HttpServer::send(int code, char *content_type, const String &content)
	{
		send(code, (const char *) content_type, content);
	}

	void HttpServer::send(int code, const String &content_type, const String &content)
	{
		send(code, content_type.c_str(), content);
	}

	void HttpServer::sendContent(const String &content)
	{
		const char *footer = "\r\n";
		size_t len = content.length();

		if(_chunked) {
			auto *chunkSize = (char *) malloc(11);
			if(chunkSize) {
#ifdef HAVE_LWIP
				sprintf(chunkSize, "%x%s", len, footer);
#else
				sprintf(chunkSize, "%lx%s", len, footer);
#endif
				_currentClientWrite(chunkSize, strlen(chunkSize));
				free(chunkSize);
			}
		}
		_currentClientWrite(content.c_str(), len);
		if(_chunked) {
			_currentClient->write(footer, 2);
			if(len == 0) {
				_chunked = false;
			}
		}
	}

	void HttpServer::_streamFileCore( size_t fileSize, const String &fileName, const String &contentType)
	{
		using namespace mime;
		setContentLength(fileSize);
		if(fileName.endsWith(String(FPSTR(mimeTable[gz].endsWith))) &&
		   contentType != String(FPSTR(mimeTable[gz].mimeType)) &&
		   contentType != String(FPSTR(mimeTable[none].mimeType))) {
			sendHeader(F("Content-Encoding"), F("gzip"));
		}
		send(200, contentType, "");
	}


	String HttpServer::arg(const String& name)
	{
		for(int i = 0; i < _currentArgCount; ++i) {
			if(_currentArgs[i].key == name)
				return _currentArgs[i].value;
		}
		return "";
	}

	String HttpServer::arg(int i)
	{
		if(i < _currentArgCount)
			return _currentArgs[i].value;
		return "";
	}

	String HttpServer::argName(int i)
	{
		if(i < _currentArgCount)
			return _currentArgs[i].key;
		return "";
	}

	int HttpServer::args()
	{
		return _currentArgCount;
	}

	bool HttpServer::hasArg(const String& name)
	{
		for(int i = 0; i < _currentArgCount; ++i) {
			if(_currentArgs[i].key == name)
				return true;
		}
		return false;
	}


	String HttpServer::header(const String& name)
	{
		for(int i = 0; i < _headerKeysCount; ++i) {
			if(_currentHeaders[i].key.equalsIgnoreCase(name))
				return _currentHeaders[i].value;
		}
		return "";
	}

	void HttpServer::collectHeaders(const char *headerKeys[],  size_t headerKeysCount)
	{
		_headerKeysCount = headerKeysCount + 1;

		delete[]_currentHeaders;
		_currentHeaders = new RequestArgument[_headerKeysCount];
		_currentHeaders[0].key = FPSTR(AUTHORIZATION_HEADER);
		for(int i = 1; i < _headerKeysCount; i++) {
			_currentHeaders[i].key = headerKeys[i - 1];
		}
	}

	String HttpServer::header(int i)
	{
		if(i < _headerKeysCount)
			return _currentHeaders[i].value;
		return "";
	}

	String HttpServer::headerName(int i)
	{
		if(i < _headerKeysCount)
			return _currentHeaders[i].key;
		return "";
	}

	int HttpServer::headers()
	{
		return _headerKeysCount;
	}

	bool HttpServer::hasHeader(const String& name)
	{
		for(int i = 0; i < _headerKeysCount; ++i) {
			if((_currentHeaders[i].key.equalsIgnoreCase(name)) && (_currentHeaders[i].value.length() > 0))
				return true;
		}
		return false;
	}

	String HttpServer::hostHeader()
	{
		return _hostHeader;
	}

	void HttpServer::onFileUpload(THandlerFunction fn)
	{
		_fileUploadHandler = fn;
	}

	void HttpServer::onNotFound(THandlerFunction fn)
	{
		_notFoundHandler = fn;
	}

	void HttpServer::_handleRequest()
	{
		bool handled = false;
		if(!_currentHandler) {
#ifdef DEBUG_ESP_HTTP_SERVER
			print_dbg("Request handler not found!\n");
#endif
		} else {
			handled = _currentHandler->handle(*this, _currentMethod, _currentUri);
		}
		if(!handled && _notFoundHandler) {
			_notFoundHandler(*this);
			handled = true;
		}
		if(!handled) {
			using namespace mime;
			send(404, String(FPSTR(mimeTable[html].mimeType)), String(F("Not found: ")) + _currentUri);
			handled = true;
		}
		if(handled) {
			_finalizeResponse();
		}
		_currentUri = "";
	}


	void HttpServer::_finalizeResponse()
	{
		if(_chunked) {
			sendContent("");
		}
	}

	String HttpServer::_responseCodeToString(int code)
	{
		switch(code) {
		case 100:
			return F("Continue");
		case 101:
			return F("Switching Protocols");
		case 200:
			return F("OK");
		case 201:
			return F("Created");
		case 202:
			return F("Accepted");
		case 203:
			return F("Non-Authoritative Information");
		case 204:
			return F("No Content");
		case 205:
			return F("Reset Content");
		case 206:
			return F("Partial Content");
		case 300:
			return F("Multiple Choices");
		case 301:
			return F("Moved Permanently");
		case 302:
			return F("Found");
		case 303:
			return F("See Other");
		case 304:
			return F("Not Modified");
		case 305:
			return F("Use Proxy");
		case 307:
			return F("Temporary Redirect");
		case 400:
			return F("Bad Request");
		case 401:
			return F("Unauthorized");
		case 402:
			return F("Payment Required");
		case 403:
			return F("Forbidden");
		case 404:
			return F("Not Found");
		case 405:
			return F("Method Not Allowed");
		case 406:
			return F("Not Acceptable");
		case 407:
			return F("Proxy Authentication Required");
		case 408:
			return F("Request Time-out");
		case 409:
			return F("Conflict");
		case 410:
			return F("Gone");
		case 411:
			return F("Length Required");
		case 412:
			return F("Precondition Failed");
		case 413:
			return F("Request Entity Too Large");
		case 414:
			return F("Request-URI Too Large");
		case 415:
			return F("Unsupported Media Type");
		case 416:
			return F("Requested range not satisfiable");
		case 417:
			return F("Expectation Failed");
		case 500:
			return F("Internal Server Error");
		case 501:
			return F("Not Implemented");
		case 502:
			return F("Bad Gateway");
		case 503:
			return F("Service Unavailable");
		case 504:
			return F("Gateway Time-out");
		case 505:
			return F("HTTP Version not supported");
		default:
			return F("");
		}
	}

	static char *readBytesWithTimeout(TcpClient &client, size_t maxLength, size_t &dataLength, int timeout_ms)
	{
		char *buf = nullptr;
		dataLength = 0;
		while(dataLength < maxLength) {
			int tries = timeout_ms;
			size_t newLength;
			while(!(newLength = client.available()) && tries--)
				lwiot_sleep(10);
			if(!newLength) {
				break;
			}
			if(!buf) {
				buf = (char *) malloc(newLength + 1);
				if(!buf) {
					return nullptr;
				}
			} else {
				auto *newBuf = (char *) realloc(buf, dataLength + newLength + 1);
				if(!newBuf) {
					free(buf);
					return nullptr;
				}
				buf = newBuf;
			}
			client.read(buf + dataLength, newLength);
			dataLength += newLength;
			buf[dataLength] = '\0';
		}
		return buf;
	}

	bool HttpServer::_parseRequest(TcpClient &client)
	{
		// Read the first line of HTTP request
		String req = client.readStringUntil('\r');
		client.readStringUntil('\n');
		//reset header value
		for(int i = 0; i < _headerKeysCount; ++i) {
			_currentHeaders[i].value = String();
		}

		// First line of HTTP request looks like "GET /path HTTP/1.1"
		// Retrieve the "/path" part by finding the spaces
		int addr_start = req.indexOf(' ');
		int addr_end = req.indexOf(' ', addr_start + 1);
		if(addr_start == -1 || addr_end == -1) {
#ifdef DEBUG_ESP_HTTP_SERVER
			DEBUG_OUTPUT.print("Invalid request: ");
	DEBUG_OUTPUT.println(req);
#endif
			return false;
		}

		String methodStr = req.substring(0, addr_start);
		String url = req.substring(addr_start + 1, addr_end);
		String versionEnd = req.substring(addr_end + 8);
		_currentVersion = atoi(versionEnd.c_str());
		String searchStr = "";
		int hasSearch = url.indexOf('?');
		if(hasSearch != -1) {
			searchStr = url.substring(hasSearch + 1);
			url = url.substring(0, hasSearch);
		}
		_currentUri = url;
		_chunked = false;

		HTTPMethod method = HTTP_GET;
		if(methodStr == F("POST")) {
			method = HTTP_POST;
		} else if(methodStr == F("DELETE")) {
			method = HTTP_DELETE;
		} else if(methodStr == F("OPTIONS")) {
			method = HTTP_OPTIONS;
		} else if(methodStr == F("PUT")) {
			method = HTTP_PUT;
		} else if(methodStr == F("PATCH")) {
			method = HTTP_PATCH;
		}
		_currentMethod = method;

#ifdef DEBUG_ESP_HTTP_SERVER
		DEBUG_OUTPUT.print("method: ");
  DEBUG_OUTPUT.print(methodStr);
  DEBUG_OUTPUT.print(" url: ");
  DEBUG_OUTPUT.print(url);
  DEBUG_OUTPUT.print(" search: ");
  DEBUG_OUTPUT.println(searchStr);
#endif

		//attach handler
		RequestHandler *handler;
		for(handler = _firstHandler; handler; handler = handler->next()) {
			if(handler->canHandle(_currentMethod, _currentUri))
				break;
		}
		_currentHandler = handler;

		String formData;
		// below is needed only when POST type request
		if(method == HTTP_POST || method == HTTP_PUT || method == HTTP_PATCH || method == HTTP_DELETE) {
			String boundaryStr;
			String headerName;
			String headerValue;
			bool isForm = false;
			bool isEncoded = false;
			uint32_t contentLength = 0;
			//parse headers
			while(true) {
				req = client.readStringUntil('\r');
				client.readStringUntil('\n');
				if(req == "")
					break;//no moar headers
				int headerDiv = req.indexOf(':');
				if(headerDiv == -1) {
					break;
				}
				headerName = req.substring(0, headerDiv);
				headerValue = req.substring(headerDiv + 1);
				headerValue.trim();
				_collectHeader(headerName.c_str(), headerValue.c_str());

#ifdef DEBUG_ESP_HTTP_SERVER
				DEBUG_OUTPUT.print("headerName: ");
	  DEBUG_OUTPUT.println(headerName);
	  DEBUG_OUTPUT.print("headerValue: ");
	  DEBUG_OUTPUT.println(headerValue);
#endif

				if(headerName.equalsIgnoreCase(FPSTR(Content_Type))) {
					using namespace mime;
					if(headerValue.startsWith(FPSTR(mimeTable[txt].mimeType))) {
						isForm = false;
					} else if(headerValue.startsWith(F("application/x-www-form-urlencoded"))) {
						isForm = false;
						isEncoded = true;
					} else if(headerValue.startsWith(F("multipart/"))) {
						boundaryStr = headerValue.substring(headerValue.indexOf('=') + 1);
						boundaryStr.replace("\"", "");
						isForm = true;
					}
				} else if(headerName.equalsIgnoreCase(F("Content-Length"))) {
					contentLength = headerValue.toInt();
				} else if(headerName.equalsIgnoreCase(F("Host"))) {
					_hostHeader = headerValue;
				}
			}

			if(!isForm) {
				size_t plainLength;
				char *plainBuf = readBytesWithTimeout(client, contentLength, plainLength, HTTP_MAX_POST_WAIT);
				if(plainLength < contentLength) {
					free(plainBuf);
					return false;
				}
				if(contentLength > 0) {
					if(isEncoded) {
						//url encoded form
						if(searchStr != "")
							searchStr += '&';
						searchStr += plainBuf;
					}
					_parseArguments(searchStr);
					if(!isEncoded) {
						//plain post json or other data
						RequestArgument &arg = _currentArgs[_currentArgCount++];
						arg.key = F("plain");
						arg.value = String(plainBuf);
					}

#ifdef DEBUG_ESP_HTTP_SERVER
					DEBUG_OUTPUT.print("Plain: ");
		DEBUG_OUTPUT.println(plainBuf);
#endif
					free(plainBuf);
				} else {
					// No content - but we can still have arguments in the URL.
					_parseArguments(searchStr);
				}
			}

			if(isForm) {
				_parseArguments(searchStr);
				if(!_parseForm(client, boundaryStr, contentLength)) {
					return false;
				}
			}
		} else {
			String headerName;
			String headerValue;

			while(true) {
				req = client.readStringUntil('\r');
				client.readStringUntil('\n');
				if(req == "")
					break;
				int headerDiv = req.indexOf(':');
				if(headerDiv == -1) {
					break;
				}
				headerName = req.substring(0, headerDiv);
				headerValue = req.substring(headerDiv + 2);
				_collectHeader(headerName.c_str(), headerValue.c_str());

				if(headerName.equalsIgnoreCase("Host")) {
					_hostHeader = headerValue;
				}
			}

			_parseArguments(searchStr);
		}

		return true;
	}

	bool HttpServer::_collectHeader(const char *headerName, const char *headerValue)
	{
		for(int i = 0; i < _headerKeysCount; i++) {
			if(_currentHeaders[i].key.equalsIgnoreCase(headerName)) {
				_currentHeaders[i].value = headerValue;
				return true;
			}
		}
		return false;
	}

	void HttpServer::_parseArguments(const String& data)
	{

		delete[] _currentArgs;

		_currentArgs = nullptr;

		if(data.length() == 0) {
			_currentArgCount = 0;
			_currentArgs = new RequestArgument[1];
			return;
		}
		_currentArgCount = 1;

		for(int i = 0; i < (int) data.length();) {
			i = data.indexOf('&', i);
			if(i == -1)
				break;
			++i;
			++_currentArgCount;
		}

		_currentArgs = new RequestArgument[_currentArgCount + 1];
		int pos = 0;
		int iarg;

		for(iarg = 0; iarg < _currentArgCount;) {
			int equal_sign_index = data.indexOf('=', pos);
			int next_arg_index = data.indexOf('&', pos);

			if((equal_sign_index == -1) || ((equal_sign_index > next_arg_index) && (next_arg_index != -1))) {
				if(next_arg_index == -1)
					break;
				pos = next_arg_index + 1;
				continue;
			}

			RequestArgument &arg = _currentArgs[iarg];
			arg.key = urlDecode(data.substring(pos, equal_sign_index));
			arg.value = urlDecode(data.substring(equal_sign_index + 1, next_arg_index));
			++iarg;

			if(next_arg_index == -1)
				break;

			pos = next_arg_index + 1;
		}
		_currentArgCount = iarg;
	}

	void HttpServer::_uploadWriteByte(uint8_t b)
	{
		if(_currentUpload->currentSize == HTTP_UPLOAD_BUFLEN) {
			if(_currentHandler && _currentHandler->canUpload(_currentUri))
				_currentHandler->upload(*this, _currentUri, *_currentUpload);
			_currentUpload->totalSize += _currentUpload->currentSize;
			_currentUpload->currentSize = 0;
		}
		_currentUpload->buf[_currentUpload->currentSize++] = b;
	}

	uint8_t HttpServer::_uploadReadByte(TcpClient &client)
	{
		int res = client.read();
		if(res == -1) {
			while(!client.available() && client.connected())
				Thread::yield();
			res = client.read();
		}
		return (uint8_t) res;
	}

	bool HttpServer::_parseForm(TcpClient &client, const String& boundary, uint32_t len)
	{
		(void) len;
		String line;
		int retry = 0;
		do {
			line = client.readStringUntil('\r');
			++retry;
		} while(line.length() == 0 && retry < 3);

		client.readStringUntil('\n');
		//start reading the form
		if(line == ("--" + boundary)) {
			auto *postArgs = new RequestArgument[32];
			int postArgsLen = 0;
			while(true) {
				String argName;
				String argValue;
				String argType;
				String argFilename;
				bool argIsFile = false;

				line = client.readStringUntil('\r');
				client.readStringUntil('\n');
				if(line.length() > 19 && line.substring(0, 19).equalsIgnoreCase(F("Content-Disposition"))) {
					int nameStart = line.indexOf('=');
					if(nameStart != -1) {
						argName = line.substring(nameStart + 2);
						nameStart = argName.indexOf('=');
						if(nameStart == -1) {
							argName = argName.substring(0, argName.length() - 1);
						} else {
							argFilename = argName.substring(nameStart + 2, argName.length() - 1);
							argName = argName.substring(0, argName.indexOf('"'));
							argIsFile = true;

							//use GET to set the filename if uploading using blob
							if(argFilename == F("blob") && hasArg(FPSTR(filename)))
								argFilename = arg(FPSTR(filename));
						}

						using namespace mime;
						argType = FPSTR(mimeTable[txt].mimeType);
						line = client.readStringUntil('\r');
						client.readStringUntil('\n');
						if(line.length() > 12 && line.substring(0, 12).equalsIgnoreCase(FPSTR(Content_Type))) {
							argType = line.substring(line.indexOf(':') + 2);
							//skip next line
							client.readStringUntil('\r');
							client.readStringUntil('\n');
						}

						if(!argIsFile) {
							while(true) {
								line = client.readStringUntil('\r');
								client.readStringUntil('\n');
								if(line.startsWith("--" + boundary))
									break;
								if(argValue.length() > 0)
									argValue += "\n";
								argValue += line;
							}

							RequestArgument &arg = postArgs[postArgsLen++];
							arg.key = argName;
							arg.value = argValue;

							if(line == ("--" + boundary + "--")) {
								break;
							}
						} else {
							_currentUpload.reset(new HTTPUpload());
							_currentUpload->status = UPLOAD_FILE_START;
							_currentUpload->name = argName;
							_currentUpload->filename = argFilename;
							_currentUpload->type = argType;
							_currentUpload->totalSize = 0;
							_currentUpload->currentSize = 0;

							if(_currentHandler && _currentHandler->canUpload(_currentUri))
								_currentHandler->upload(*this, _currentUri, *_currentUpload);
							_currentUpload->status = UPLOAD_FILE_WRITE;
							uint8_t argByte = _uploadReadByte(client);
readfile:
							while(argByte != 0x0D) {
								if(!client.connected())
									return _parseFormUploadAborted();
								_uploadWriteByte(argByte);
								argByte = _uploadReadByte(client);
							}

							argByte = _uploadReadByte(client);
							if(!client.connected())
								return _parseFormUploadAborted();
							if(argByte == 0x0A) {
								argByte = _uploadReadByte(client);
								if(!client.connected())
									return _parseFormUploadAborted();
								if((char) argByte != '-') {
									//continue reading the file
									_uploadWriteByte(0x0D);
									_uploadWriteByte(0x0A);
									goto readfile;
								} else {
									argByte = _uploadReadByte(client);
									if(!client.connected())
										return _parseFormUploadAborted();
									if((char) argByte != '-') {
										//continue reading the file
										_uploadWriteByte(0x0D);
										_uploadWriteByte(0x0A);
										_uploadWriteByte((uint8_t) ('-'));
										goto readfile;
									}
								}


								ByteBuffer endBuf(boundary.length());

								client.read(endBuf.data(), boundary.length());
								endBuf.setIndex(boundary.length());

								if(strstr((const char *) endBuf.data(), boundary.c_str()) != nullptr) {
									if(_currentHandler && _currentHandler->canUpload(_currentUri))
										_currentHandler->upload(*this, _currentUri, *_currentUpload);
									_currentUpload->totalSize += _currentUpload->currentSize;
									_currentUpload->status = UPLOAD_FILE_END;
									if(_currentHandler && _currentHandler->canUpload(_currentUri))
										_currentHandler->upload(*this, _currentUri, *_currentUpload);

									line = client.readStringUntil(0x0D);
									client.readStringUntil(0x0A);
									if(line == "--") {
										break;
									}

									continue;
								} else {
									_uploadWriteByte(0x0D);
									_uploadWriteByte(0x0A);
									_uploadWriteByte((uint8_t) ('-'));
									_uploadWriteByte((uint8_t) ('-'));
									uint32_t i = 0;
									while(i < boundary.length()) {
										_uploadWriteByte(endBuf[i++]);
									}
									argByte = _uploadReadByte(client);
									goto readfile;
								}
							} else {
								_uploadWriteByte(0x0D);
								goto readfile;
							}
						}
					}
				}
			}

			int iarg;
			int totalArgs = ((32 - postArgsLen) < _currentArgCount) ? (32 - postArgsLen) : _currentArgCount;
			for(iarg = 0; iarg < totalArgs; iarg++) {
				RequestArgument &arg = postArgs[postArgsLen++];
				arg.key = _currentArgs[iarg].key;
				arg.value = _currentArgs[iarg].value;
			}

			delete[] _currentArgs;
			_currentArgs = new RequestArgument[postArgsLen];
			for(iarg = 0; iarg < postArgsLen; iarg++) {
				RequestArgument &arg = _currentArgs[iarg];
				arg.key = postArgs[iarg].key;
				arg.value = postArgs[iarg].value;
			}
			_currentArgCount = iarg;
			delete[] postArgs;
			return true;
		}

		return false;
	}

	String HttpServer::urlDecode(const String &text)
	{
		String decoded = "";
		char temp[] = "0x00";
		unsigned int len = text.length();
		unsigned int i = 0;
		while(i < len) {
			char decodedChar;
			char encodedChar = text.charAt(i++);
			if((encodedChar == '%') && (i + 1 < len)) {
				temp[2] = text.charAt(i++);
				temp[3] = text.charAt(i++);

				decodedChar = strtol(temp, nullptr, 16);
			} else {
				if(encodedChar == '+') {
					decodedChar = ' ';
				} else {
					decodedChar = encodedChar;  // normal ascii char
				}
			}
			decoded += decodedChar;
		}
		return decoded;
	}

	bool HttpServer::_parseFormUploadAborted()
	{
		_currentUpload->status = UPLOAD_FILE_ABORTED;
		if(_currentHandler && _currentHandler->canUpload(_currentUri))
			_currentHandler->upload(*this, _currentUri, *_currentUpload);
		return false;
	}
}

