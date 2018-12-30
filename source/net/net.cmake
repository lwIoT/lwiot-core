if(UNIX)
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

	net/udp/udpclient.cpp
	net/udp/udpserver.cpp
	net/udp/socketudpclient.cpp
	net/udp/socketudpserver.cpp

	net/util/base64.c
	net/util/captiveportal.cpp
	net/util/ipaddress.cpp

	net/802.15.4/xbee.cpp
	net/802.15.4/xbeeresponse.cpp
	net/802.15.4/xbeerequest.cpp

	net/http/httpserver.cpp
	net/http/mimetable.cpp
	net/http/mimetable.h
	net/http/requesthandlerimpl.h
)
