/*
 * C#-style DateTime implementation.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file datetime.h

#pragma once

#include <lwiot.h>
#include <string.h>
#include <stdlib.h>

#include <lwiot/types.h>
#include <lwiot/stl/string.h>

namespace lwiot
{
	/**
	 * @brief Time units.
	 * @ingroup util
	 */
	typedef enum {
		MILLISECOND,
		SECOND,
		MINUTE,
		HOUR,
		DAY,
		MONTH,
		YEAR
	} time_unit_t;

	/**
	 * @brief Date/time wrapper class.
	 * @ingroup util
	 */
	class DateTime {
	public:
		/**
		 * @brief Construct a new date time object.
		 * @note The initial value is the current time stamp.
		 */
		explicit DateTime();

		/**
		 * @brief Copy constructor.
		 * @param dt Date/time to copy.
		 */
		DateTime(const DateTime& dt);
		/**
		 * @brief Construct a new date/time object.
		 * @param ts Initial value.
		 */
		explicit DateTime(const time_t& ts);

		/**
		 * @brief Move constructor.
		 * @param dt Date/time to move.
		 */
		DateTime(DateTime&& dt) = default;

		/**
		 * @brief Move assignment operator.
		 * @param dt Date/time to move.
		 */
		DateTime& operator=(DateTime&& dt) = default;

		/**
		 * @brief Destruct a date/time object.
		 */
		virtual ~DateTime() = default;

		/**
		 * @brief Create a date/time object for the current date & time.
		 */
		static DateTime now();

		/**
		 * @brief Convert to a String object.
		 * @return A string representation of \p *this.
		 */
		String toString() const;

		/**
		 * @brief Set the date/time to \p value.
		 * @param value Timestamp to set.
		 */
		void sync(const time_t& value);

		/**
		 * @brief Update the time value.
		 */
		void update();

		/**
		 * @brief Get the timestamp value.
		 * @return The timestamp value of \p *this.
		 */
		const time_t& timestamp() const;

		/**
		 * @brief Get the year.
		 * @return The year.
		 */
		int year() const;
		/**
		 * @brief Get the month.
		 * @return The month.
		 */
		int month() const;
		/**
		 * @brief Get the day.
		 * @return The day.
		 */
		int day() const;
		/**
		 * @brief Get the day of the week.
		 * @return The day of the week.
		 */
		int dayOfWeek() const;
		/**
		 * @brief Get the hour.
		 * @return The hour.
		 */
		int hour() const;
		/**
		 * @brief Get the minute.
		 * @return The minute.
		 */
		int minute() const;
		/**
		 * @brief Get the second.
		 * @return The second.
		 */
		int second() const;

		/**
		 * @brief Copy assignment operator.
		 * @param time Date/time object to assign.
		 * @return A reference to \p *this.
		 */
		DateTime& operator=(const DateTime& time);

		/**
		 * @brief Timestamp conversion operator.
		 * @see timestamp
		 * @return The timestamp value of \p *this.
		 */
		operator time_t() const;

		/* Equality operators */
		/**
		 * @brief Compare \p other against \p *this.
		 * @param other Date/time object to compare against \p *this.
		 * @return A reference to \p *this.
		 */
		bool operator==(const DateTime& other) const;
		/**
		 * @brief Compare \p other against \p *this.
		 * @param other Date/time object to compare against \p *this.
		 * @return A reference to \p *this.
		 */
		bool operator!=(const DateTime& other) const;
		/**
		 * @brief Compare \p other against \p *this.
		 * @param other Date/time object to compare against \p *this.
		 * @return A reference to \p *this.
		 */
		bool operator<(const DateTime& other) const;
		/**
		 * @brief Compare \p other against \p *this.
		 * @param other Date/time object to compare against \p *this.
		 * @return A reference to \p *this.
		 */
		bool operator>(const DateTime& other) const;
		/**
		 * @brief Compare \p other against \p *this.
		 * @param other Date/time object to compare against \p *this.
		 * @return A reference to \p *this.
		 */
		bool operator<=(const DateTime& other) const;
		/**
		 * @brief Compare \p other against \p *this.
		 * @param other Date/time object to compare against \p *this.
		 * @return A reference to \p *this.
		 */
		bool operator>=(const DateTime& other) const;

		/**
		 * @brief Add \p time to \p *this.
		 * @param time Time to add to \p *this.
		 * @return \p *this
		 */
		DateTime operator+(const DateTime& time) const;
		/**
		 * @brief Add \p time to \p *this.
		 * @param time Time to add to \p *this.
		 * @return \p *this
		 */
		DateTime operator+(const time_t& time) const;
		/**
		 * @brief Add \p time to \p *this.
		 * @param time Time to add to \p *this.
		 * @return A reference to \p *this
		 */
		DateTime& operator+=(const DateTime& time);
		/**
		 * @brief Add \p time to \p *this.
		 * @param other Time to add to \p *this.
		 * @return A reference to \p *this
		 */
		DateTime& operator+=(const time_t& other);

		/**
		 * @brief Substract \p time to \p *this.
		 * @param time Time to add to \p *this.
		 * @return \p *this
		 */
		DateTime operator-(const DateTime& time) const;
		/**
		 * @brief Substract \p time to \p *this.
		 * @param other Time to add to \p *this.
		 * @return \p *this
		 */
		DateTime operator-(const time_t& other) const;
		/**
		 * @brief Substract \p time to \p *this.
		 * @param time Time to add to \p *this.
		 * @return A reference to \p *this
		 */
		DateTime& operator-=(const DateTime& time);
		/**
		 * @brief Substract \p time to \p *this.
		 * @param other Time to add to \p *this.
		 * @return A reference to \p *this
		 */
		DateTime& operator-=(const time_t& other);

	private:
		time_t stamp;
		mutable struct tm gmt;

		/* Methods */
		void updateGmt() const;
		int compare(const time_t& other) const;
	};
}
