/*
 * HTTP server.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file httpserver.h

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/log.h>
#include <lwiot/stl/string.h>
#include <lwiot/stream.h>
#include <lwiot/network/requesthandler.h>
#include <lwiot/function.h>

#include <lwiot/network/ipaddress.h>
#include <lwiot/network/tcpserver.h>
#include <lwiot/network/tcpclient.h>
#include <lwiot/uniquepointer.h>

namespace lwiot
{
	/**
	 * @brief HTTP server class.
	 * @ingroup net
	 */
	class HttpServer {
	public:
		explicit HttpServer(TcpServer* server); //!< Construct a new HTTP server.
		virtual ~HttpServer(); //!< HTTP server destructor.

		virtual bool begin(); //!< Start a HTTP server.

		virtual void handleClient(); //!< Handle a single HTTP client.
		virtual void close(); //!< Stop the HTTP server socket.

		void stop(); //!< Stop a HTTP server socket. Alias for HttpServer::close.

		/**
		 * @brief Authenticate a client.
		 * @param mode Authentication method.
		 * @param realm Authentication realm.
		 * @param authFailMsg Failed authentication message.
		 */
		void requestAuthentication(HTTPAuthMethod mode = BASIC_AUTH, const char *realm = nullptr,
		                           const String &authFailMsg = String(""));

		using THandlerFunction = Function<void(HttpServer& server)>; //!< Handler function.

		/**
		 * @brief Add handler.
		 * @param uri URI to add a handler for.
		 * @param handler Handler object.
		 */
		void on(const String &uri, THandlerFunction handler);
		/**
		 * @brief Add handler.
		 * @param uri URI to add a handler for.
		 * @param method HTTP method.
		 * @param fn HTTP method.
		 */
		void on(const String &uri, HTTPMethod method, THandlerFunction fn);
		/**
		 * @brief Add handler.
		 * @param uri URI to add a handler for.
		 * @param method HTTP method.
		 * @param fn Handler object.
		 * @param ufn Upload handler.
		 */
		void on(const String &uri, HTTPMethod method, THandlerFunction fn, THandlerFunction ufn);

		void addHandler(RequestHandler *handler); //!< Add a request handler.

		void onNotFound(THandlerFunction fn);  //!< Add a 404 handler.
		void onFileUpload(THandlerFunction fn); //!< Add a file upload handler.

		/**
		 * @brief Get the current URI.
		 * @return The current URI.
		 */
		String uri()
		{
			return _currentUri;
		}

		/**
		 * @brief Get the current HTTP method.
		 * @return The current HTTP method.
		 */
		HTTPMethod method()
		{
			return _currentMethod;
		}

		/**
		 * @brief Get the current upload.
		 * @return The current HTTP upload data.
		 */
		HTTPUpload &upload()
		{
			return *_currentUpload;
		}

		bool hasClient() const; //!< Check if there is a current client.
		String arg(const String& name);        //!< Get request argument value by name.
		String arg(int i);              //!< Get request argument value by number.
		String argName(int i);          //!< Get request argument name by number.
		int args();                     //!< Get arguments count.
		bool hasArg(const String& name);       //!< Check if argument exists.
		void collectHeaders(const char *headerKeys[], size_t headerKeysCount); //!< Set the request headers to collect.
		String header(const String& name);      //!< Get request header value by name.
		String header(int i);              //!< Get request header value by number.
		String headerName(int i);          //!< Get request header name by number.
		int headers();                     //!< Get header count.
		bool hasHeader(const String& name);       //!< Check if header exists.

		String hostHeader();            //!< Get request host header if available or empty String if not.

		/**
		 * @brief Send a HTTP packet.
		 * @param code HTTP code.
		 * @param content_type Content type.
		 * @param content Content.
		 */
		void send(int code, const char *content_type = nullptr, const String &content = String(""));

		/**
		 * @brief Send a HTTP packet.
		 * @param code HTTP code.
		 * @param content_type Content type.
		 * @param content Content.
		 */
		void send(int code, char *content_type, const String &content);

		/**
		 * @brief Send a HTTP packet.
		 * @param code HTTP code.
		 * @param content_type Content type.
		 * @param content Content.
		 */
		void send(int code, const String &content_type, const String &content);


		void setContentLength(size_t contentLength); //!< Set the content length.

		/**
		 * @brief Send a HTTP header.
		 * @param name Header name.
		 * @param value Header value.
		 * @param first First header indicator.
		 */
		void sendHeader(const String &name, const String &value, bool first = false);

		/**
		 * @brief Send HTTP content.
		 * @param content Content to send.
		 */
		void sendContent(const String &content);

		static String urlDecode(const String &text); //!< Decode an URL.

		/**
		 * @brief Stream the contents of a file.
		 * @tparam T File type.
		 * @param file File to stream.
		 * @param contentType Content type.
		 * @return Number of bytes written to the underlying TCP stream.
		 */
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
		void _parseArguments(const String& data);

		static String _responseCodeToString(int code);

		bool _parseForm(TcpClient &client, const String& boundary, uint32_t len);
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
