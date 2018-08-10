/*
 * C#-style DateTime implementation.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <lwiot.h>
#include <stdlib.h>
#include <time.h>

#include <lwiot/string.h>

namespace lwiot
{
	typedef enum {
		MILLISECOND,
		SECOND,
		MINUTE,
		HOUR,
		DAY,
		MONTH,
		YEAR
	} time_unit_t;

	class DateTime {
	public:
		explicit DateTime();
		explicit DateTime(const DateTime& dt);
		DateTime(const time_t& ts);

		virtual ~DateTime() = default;

		static DateTime now();

		String toString() const;
		void sync(const time_t& value);
		void update();

		const time_t& timestamp() const;
		int year() const;
		int month() const;
		int day() const;
		int dayOfWeek() const;
		int hour() const;
		int minute() const;
		int second() const;

		DateTime& operator=(const DateTime& time);
		operator time_t() const;

		/* Equality operators */
		bool operator==(const DateTime& other) const;
		bool operator!=(const DateTime& other) const;
		bool operator<(const DateTime& other) const;
		bool operator>(const DateTime& other) const;
		bool operator<=(const DateTime& other) const;
		bool operator>=(const DateTime& other) const;

		DateTime operator+(const DateTime& time) const;
		DateTime operator+(const time_t& other) const;
		DateTime& operator+=(const DateTime& time);
		DateTime& operator+=(const time_t& other);

		DateTime operator-(const DateTime& time) const;
		DateTime operator-(const time_t& other) const;
		DateTime& operator-=(const DateTime& time);
		DateTime& operator-=(const time_t& other);

	private:
		time_t stamp;
		mutable struct tm gmt;

		/* Methods */
		void updateGmt() const;
		void add(const time_t& other);
		int compare(const time_t& other) const;
	};
}
