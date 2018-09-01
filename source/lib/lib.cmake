
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

if(HAVE_LIBTIME)
SET(TIME_SOURCES
	lib/time/asctime.c
	lib/time/asctime_r.c
	lib/time/asc_store.c
	lib/time/ctime.c
	lib/time/ctime_r.c
	lib/time/daylight_seconds.c
	lib/time/difftime.c
	lib/time/dst_pointer.c
	lib/time/equation_of_time.c
	lib/time/fatfs_time.c
	lib/time/geo_location.c
	lib/time/gmtime.c
	lib/time/gmtime_r.c
	lib/time/gm_sidereal.c
	lib/time/isotime.c
	lib/time/isotime_r.c
	lib/time/iso_week_date.c
	lib/time/iso_week_date_r.c
	lib/time/is_leap.c
	lib/time/lm_sidereal.c
	lib/time/localtime.c
	lib/time/localtime_r.c
	lib/time/mktime.c
	lib/time/mk_gmtime.c
	lib/time/month_length.c
	lib/time/moon_phase.c
	lib/time/print_lz.c
	lib/time/set_dst.c
	lib/time/set_position.c
	lib/time/set_system_time.c
	lib/time/set_zone.c
	lib/time/solar_declination.c
	lib/time/solar_noon.c
	lib/time/strftime.c
	lib/time/sun_rise.c
	lib/time/sun_set.c
	lib/time/system_time.c
	lib/time/time.c
	lib/time/tm_store.c
	lib/time/utc_offset.c
	lib/time/week_of_month.c
	lib/time/week_of_year.c
	lib/time/system_tick.S
)
endif()

SET(LIB_SOURCES
	lib/streams/bufferedstream.cpp
	lib/streams/stream.cpp
	lib/streams/printer.cpp

	lib/gfx/gfxbase.cpp
	lib/gfx/ssd1306display.cpp

	lib/net/ipaddress.cpp

	lib/count.cpp

	${JSON_SOURCES}
	${TIME_SOURCES}
)