/*
 * HTTP request handler.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

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
#include <lwiot/network/httpserver.h>

#include "mimetable.h"

using namespace mime;
namespace lwiot
{
	class FunctionRequestHandler : public RequestHandler {
	public:
		FunctionRequestHandler(const HttpServer::THandlerFunction& fn, const HttpServer::THandlerFunction& ufn,
		                       const String &uri, HTTPMethod method)
				: _fn(fn), _ufn(ufn), _uri(uri), _method(method)
		{
		}

		bool canHandle(HTTPMethod requestMethod, String requestUri) override
		{
			if(_method != HTTP_ANY && _method != requestMethod)
				return false;

			return (requestUri != _uri) == 0;

		}

		bool canUpload(String requestUri) override
		{
			return !(!_ufn || !canHandle(HTTP_POST, requestUri));

		}

		bool handle(HttpServer &server, HTTPMethod requestMethod, String requestUri) override
		{
			if(!canHandle(requestMethod, requestUri))
				return false;

			_fn(server);
			return true;
		}

		void upload(HttpServer &server, String requestUri, HTTPUpload &upload) override
		{
			if(canUpload(requestUri))
				_ufn(server);
		}

	protected:
		HttpServer::THandlerFunction _fn;
		HttpServer::THandlerFunction _ufn;
		String _uri;
		HTTPMethod _method;
	};
}
