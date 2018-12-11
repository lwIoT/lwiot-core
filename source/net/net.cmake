if(HAVE_LWIP)
	SET(SOCKETS net/sockets/lwip.c)
elseif(UNIX)
	SET(SOCKETS net/sockets/unix.c)
elseif(WIN32)
	SET(SOCKETS net/sockets/win32.c)
endif()

SET(NET_SOURCES
	${SOCKETS}
	net/tcp/tcpclient.cpp
	net/tcp/sockettcpclient.cpp
	net/tcp/tcpserver.cpp
	net/tcp/sockettcpserver.cpp
	#net/dns.c
	net/util/base64.c

	net/http/httpserver.cpp
	net/http/mimetable.cpp
	net/http/mimetable.h
	net/http/requesthandlerimpl.h
)