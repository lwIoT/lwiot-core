/*
 * HTTP request handler.
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
#include <lwiot/stl/string.h>
#include <lwiot/stream.h>
#include <lwiot/network/ipaddress.h>
#include <lwiot/network/tcpserver.h>
#include <lwiot/network/tcpclient.h>

namespace lwiot
{
	enum HTTPMethod {
		HTTP_ANY, HTTP_GET, HTTP_POST, HTTP_PUT, HTTP_PATCH, HTTP_DELETE, HTTP_OPTIONS
	};
	enum HTTPUploadStatus {
		UPLOAD_FILE_START, UPLOAD_FILE_WRITE, UPLOAD_FILE_END,
		UPLOAD_FILE_ABORTED
	};
	enum HTTPClientStatus {
		HC_NONE, HC_WAIT_READ, HC_WAIT_CLOSE
	};
	enum HTTPAuthMethod {
		BASIC_AUTH, DIGEST_AUTH
	};

	class HttpServer;

#define HTTP_DOWNLOAD_UNIT_SIZE 1460

#ifndef HTTP_UPLOAD_BUFLEN
#define HTTP_UPLOAD_BUFLEN 2048
#endif

#define HTTP_MAX_DATA_WAIT 5000 //ms to wait for the client to send the request
#define HTTP_MAX_POST_WAIT 5000 //ms to wait for POST data to arrive
#define HTTP_MAX_SEND_WAIT 5000 //ms to wait for data chunk to be ACKed
#define HTTP_MAX_CLOSE_WAIT 2000 //ms to wait for the client to close the connection

#define CONTENT_LENGTH_UNKNOWN ((size_t) -1)
#define CONTENT_LENGTH_NOT_SET ((size_t) -2)

	typedef struct {
		HTTPUploadStatus status;
		String filename;
		String name;
		String type;
		size_t totalSize;    // file size
		size_t currentSize;  // size of data currently in buf
		uint8_t buf[HTTP_UPLOAD_BUFLEN];
	} HTTPUpload;

	class RequestHandler {
	public:
		virtual ~RequestHandler()
		{
		}

		virtual bool canHandle(HTTPMethod method, String uri)
		{
			(void) method;
			(void) uri;
			return false;
		}

		virtual bool canUpload(String uri)
		{
			(void) uri;
			return false;
		}

		virtual bool handle(HttpServer &server, HTTPMethod requestMethod, String requestUri)
		{
			(void) server;
			(void) requestMethod;
			(void) requestUri;
			return false;
		}

		virtual void upload(HttpServer &server, String requestUri, HTTPUpload &upload)
		{
			(void) server;
			(void) requestUri;
			(void) upload;
		}

		RequestHandler *next()
		{
			return _next;
		}

		void next(RequestHandler *r)
		{
			_next = r;
		}

	private:
		RequestHandler *_next = nullptr;
	};
}
