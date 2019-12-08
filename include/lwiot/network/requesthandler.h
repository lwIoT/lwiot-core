/*
 * HTTP request handler.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file requesthandler.h

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
	/**
	 * @brief HTTP methods.
	 */
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

	/**
	 * @brief Authentication methods.
	 */
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
#define HTTP_MAX_SEND_WAIT 5 //ms to wait for data chunk to be ACKed
#define HTTP_MAX_CLOSE_WAIT 2000 //ms to wait for the client to close the connection

#define CONTENT_LENGTH_UNKNOWN ((size_t) -1)
#define CONTENT_LENGTH_NOT_SET ((size_t) -2)

	/**
	 * @brief HTTP upload data.
	 */
	typedef struct {
		HTTPUploadStatus status;
		String filename;
		String name;
		String type;
		size_t totalSize;    // file size
		size_t currentSize;  // size of data currently in buf
		uint8_t buf[HTTP_UPLOAD_BUFLEN];
	} HTTPUpload;

	/**
	 * @brief Request handler.
	 */
	class RequestHandler {
	public:
		virtual ~RequestHandler() = default;

		/**
		 * @brief Check if a method and URI can be handled.
		 * @param method Method to check.
		 * @param uri URI to check.
		 * @return Ability indicator.
		 */
		virtual bool canHandle(HTTPMethod method, String uri)
		{
			(void) method;
			(void) uri;
			return false;
		}

		/**
		 * @brief Check if an upload can be handled.
		 * @param uri URI to check.
		 * @return Ability indicator.
		 */
		virtual bool canUpload(String uri)
		{
			(void) uri;
			return false;
		}

		/**
		 * @brief Handle a HTTP request.
		 * @param server HTTP server.
		 * @param requestMethod Method to check.
		 * @param requestUri URI to handle.
		 */
		virtual bool handle(HttpServer &server, HTTPMethod requestMethod, String requestUri)
		{
			(void) server;
			(void) requestMethod;
			(void) requestUri;
			return false;
		}

		/**
		 * @brief Handle a HTTP upload.
		 * @param server HTTP server.
		 * @param requestUri URI to handle.
		 * @param upload Upload data.
		 */
		virtual void upload(HttpServer &server, String requestUri, HTTPUpload &upload)
		{
			(void) server;
			(void) requestUri;
			(void) upload;
		}

		/**
		 * @brief Get the next HTTP handler.
		 * @return Thenext HTTP handler.
		 */
		RequestHandler *next()
		{
			return _next;
		}

		/**
		 * @brief Set the next handler.
		 * @param r The next HTTP handler.
		 */
		void next(RequestHandler *r)
		{
			_next = r;
		}

	private:
		RequestHandler *_next = nullptr;
	};
}
