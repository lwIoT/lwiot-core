
if(HAVE_JSON)
SET(JSON_SOURCES
	lib/json/jsonarray.cpp
	lib/json/jsonbuffer.cpp
	lib/json/jsonobject.cpp
	lib/json/jsonvariant.cpp

	lib/json/comments.cpp
	lib/json/encoding.cpp
	lib/json/indentedprint.cpp
	lib/json/jsonparser.cpp
	lib/json/list.cpp
	lib/json/prettyfier.cpp
	lib/json/staticstringbuilder.cpp
	lib/json/printer.cpp
)
endif()

SET(LIB_SOURCES
	lib/streams/bufferedstream.cpp
	lib/streams/stream.cpp

	lib/net/ipaddress.cpp
	${JSON_SOURCES}
)