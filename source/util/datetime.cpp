/*
 * C#-style DateTime implementation.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <lwiot.h>

#include <lwiot/stl/string.h>
#include <lwiot/types.h>
#include <lwiot/util/datetime.h>

namespace lwiot
{
	DateTime::DateTime() : DateTime(time(NULL))
	{
	}

	DateTime::DateTime(const DateTime& dt) : DateTime(dt.stamp)
	{
	}

	DateTime::DateTime(const time_t& ts) : stamp(ts)
	{
	}

	DateTime DateTime::now()
	{
		return DateTime(time(NULL));
	}

	DateTime::operator time_t() const
	{
		return this->stamp;
	}

	DateTime& DateTime::operator=(const DateTime& time)
	{
		this->stamp = time.stamp;
		return *this;
	}

	const time_t& DateTime::timestamp() const
	{
		return this->stamp;
	}

	int DateTime::year() const
	{
		this->updateGmt();
		return this->gmt.tm_year + 1900;
	}

	int DateTime::month() const
	{
		this->updateGmt();
		return this->gmt.tm_mon;
	}

	int DateTime::day() const
	{
		this->updateGmt();
		return this->gmt.tm_mday;
	}

	int DateTime::dayOfWeek() const
	{
		this->updateGmt();
		return this->gmt.tm_wday;
	}

	int DateTime::hour() const
	{
		this->updateGmt();
		return this->gmt.tm_hour;
	}

	int DateTime::minute() const
	{
		this->updateGmt();
		return this->gmt.tm_min;
	}

	int DateTime::second() const
	{
		this->updateGmt();
		return this->gmt.tm_sec;
	}

	String DateTime::toString() const
	{
		char buffer[32];

		this->updateGmt();
		memset((void*)buffer, 0, sizeof(buffer));
		sprintf(&buffer[0], "%02i:%02i:%02i %02i-%02i-%i",
			gmt.tm_hour, gmt.tm_min, gmt.tm_sec,
			gmt.tm_mday, gmt.tm_mon+1, gmt.tm_year + 1900
		);

		return String(buffer);
	}

	void DateTime::sync(const time_t& value)
	{
		this->stamp = value;
		this->updateGmt();
	}

	void DateTime::update()
	{
		this->sync(time(NULL));
	}

	void DateTime::updateGmt() const
	{
#ifdef WIN32
		_gmtime64_s(&this->gmt, &this->stamp);
#else
		gmtime_r(&this->stamp, &this->gmt);
#endif
	}

	DateTime DateTime::operator+(const DateTime& dt) const
	{
		DateTime retval(*this);

		retval.stamp += dt.stamp;
		return retval;
	}

	DateTime DateTime::operator+(const time_t& dt) const
	{
		DateTime retval(*this);

		retval.stamp += dt;
		return retval;
	}

	DateTime& DateTime::operator+=(const DateTime& dt)
	{
		this->stamp += dt.stamp;
		return *this;
	}

	DateTime& DateTime::operator+=(const time_t& dt)
	{
		this->stamp += dt;
		return *this;
	}

	DateTime DateTime::operator-(const DateTime& dt) const
	{
		DateTime retval(*this);

		retval.stamp -= dt.stamp;
		return retval;
	}

	DateTime DateTime::operator-(const time_t& dt) const
	{
		DateTime retval(*this);

		retval.stamp -= dt;
		return retval;
	}

	DateTime& DateTime::operator-=(const DateTime& dt)
	{
		this->stamp -= dt.stamp;
		return *this;
	}

	DateTime& DateTime::operator-=(const time_t& dt)
	{
		this->stamp -= dt;
		return *this;
	}

	/*
	 * Comparison operators
	 */

	int DateTime::compare(const time_t& other) const
	{
		if(this->stamp < other)
			return -1;

		if(this->stamp > other)
			return 1;

		return 0;
	}

	bool DateTime::operator<(const DateTime& other) const
	{
		return this->compare(other.stamp) < 0;
	}

	bool DateTime::operator<=(const DateTime& other) const
	{
		return this->compare(other.stamp) <= 0;
	}

	bool DateTime::operator>(const DateTime& other) const
	{
		return this->compare(other.stamp) > 0;
	}

	bool DateTime::operator>=(const DateTime& other) const
	{
		return this->compare(other.stamp) >= 0;
	}

	bool DateTime::operator!=(const DateTime& other) const
	{
		return this->compare(other.stamp) != 0;
	}

	bool DateTime::operator==(const DateTime& other) const
	{
		return this->compare(other.stamp) == 0;
	}
}
