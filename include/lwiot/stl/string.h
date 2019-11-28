/*
 * lwIoT string wrapper.
 *
 * @author Michel Megens, Arduino
 * @email  dev@bietje.net
 */

/// @file string.h String header.

#pragma once

#include <lwiot.h>
#include <string.h>

#include <lwiot/bytebuffer.h>

#ifdef __cplusplus
namespace lwiot
{
	namespace stl
	{
		class StringSumHelper;

		/**
		 * @ingroup stl
		 * @brief Character sequence wrapper.
		 */
		class String {
			// use a function pointer to allow for "if (s)" without the
			// complications of an operator bool(). for more information, see:
			// http://www.artima.com/cppsource/safebool.html
			typedef void (String::*StringIfHelperType)() const;

			void StringIfHelper() const
			{
			}

		public:
			/**
			 * @brief Construct a new string.
			 * @param cstr String to copy.
			 */
			String(const char *cstr = "");
			/**
			 * @brief Construct a new string.
			 * @param buf Byte buffer to copy.
			 */
			explicit String(const lwiot::ByteBuffer& buf);

			/**
			 * @brief Copy constructor.
			 * @param str String to copy.
			 */
			String(const String &str);

			/**
			 * @brief Move constructor.
			 * @param rval Value to move.
			 */
			String(String &&rval) noexcept;
			explicit String(StringSumHelper &&rval);

			/**
			 * @brief Construct a new string.
			 * @param c Initial value.
			 */
			explicit String(char c);

			/**
			 * @brief Construct a new string.
			 * @param c Initial value.
			 * @param base Integer base.
			 */
			explicit String(unsigned char c, unsigned char base = 10);

			/**
			 * @brief Construct a new string.
			 * @param c Initial value.
			 * @param base Integer base.
			 */
			explicit String(int c, unsigned char base = 10);

			/**
			 * @brief Construct a new string.
			 * @param c Initial value.
			 * @param base Integer base.
			 */
			explicit String(unsigned int c, unsigned char base = 10);

			/**
			 * @brief Construct a new string.
			 * @param c Initial value.
			 * @param base Integer base.
			 */
			explicit String(long c, unsigned char base = 10);

			/**
			 * @brief Construct a new string.
			 * @param c Initial value.
			 * @param base Integer base.
			 */
			explicit String(unsigned long c, unsigned char base = 10);

			/**
			 * @brief Construct a new string.
			 * @param c Initial value.
			 * @param decimalPlaces Number of decimal places.
			 */
			explicit String(float c, unsigned char decimalPlaces = 2);

			/**
			 * @brief Construct a new string.
			 * @param c Initial value.
			 * @param decimalPlaces Number of decimal places.
			 */
			explicit String(double c, unsigned char decimalPlaces = 2);

			/**
			 * @brief Destruct a string.
			 */
			virtual ~String();

			/**
			 * @brief Reserve memory.
			 * @param size Number of characters to reserve memory for.
			 * @return True on success, false otherwise.
			 */
			bool reserve(unsigned int size);

			/**
			 * @brief Get the length of a string.
			 * @return
			 */
			size_t length()  const noexcept
			{
				return this->len;
			}

			/**
			 * @brief String copy assignment.
			 * @param rhs String to copy.
			 * @return A reference to *this.
			 */
			String &operator=(const String &rhs);

			/**
			 * @brief String copy assignment.
			 * @param cstr String to copy.
			 * @return A reference to *this.
			 */
			String &operator=(const char *cstr);


			/**
			 * @brief String move assignment.
			 * @param rval String to copy.
			 * @return A reference to *this.
			 */
			String &operator=(String &&rval) noexcept;

			String &operator=(StringSumHelper &&rval);

			/**
			 * @brief Concatinate *this with \p param.
			 * @param param Value to concatinate to \p *this.
			 * @return True on success, false otherwise.
			 */
			bool concat(const String &param);

			/**
			 * @brief Concatinate *this with \p param.
			 * @param param Value to concatinate to \p *this.
			 * @return True on success, false otherwise.
			 */
			bool concat(const char *param);

			/**
			 * @brief Concatinate *this with \p param.
			 * @param param Value to concatinate to \p *this.
			 * @return True on success, false otherwise.
			 */
			bool concat(char param);

			/**
			 * @brief Concatinate *this with \p param.
			 * @param param Value to concatinate to \p *this.
			 * @return True on success, false otherwise.
			 */
			bool concat(unsigned char param);

			/**
			 * @brief Concatinate *this with \p param.
			 * @param num Value to concatinate to \p *this.
			 * @return True on success, false otherwise.
			 */
			bool concat(int num);

			/**
			 * @brief Concatinate *this with \p param.
			 * @param num Value to concatinate to \p *this.
			 * @return True on success, false otherwise.
			 */
			bool concat(unsigned int num);

			/**
			 * @brief Concatinate *this with \p param.
			 * @param num Value to concatinate to \p *this.
			 * @return True on success, false otherwise.
			 */
			bool concat(long num);

			/**
			 * @brief Concatinate *this with \p param.
			 * @param num Value to concatinate to \p *this.
			 * @return True on success, false otherwise.
			 */
			bool concat(unsigned long num);

			/**
			 * @brief Concatinate *this with \p param.
			 * @param num Value to concatinate to \p *this.
			 * @return True on success, false otherwise.
			 */
			bool concat(float num);

			/**
			 * @brief Concatinate *this with \p param.
			 * @param num Value to concatinate to \p *this.
			 * @return True on success, false otherwise.
			 */
			bool concat(double num);

			/**
			 * @brief Compare *this against \p s.
			 * @param s String to compare against.
			 * @return True when \p s and \p *this are equal, false otherse.
			 */
			bool equalsConstantTime(const String &s) const;

			/**
			 * @brief Append \p rhs to \p *this.
			 * @param rhs Value to append to \p *this.
			 * @return A reference to \p *this.
			 */
			String &operator+=(const String &rhs)
			{
				concat(rhs);
				return (*this);
			}

			/**
			 * @brief Append \p cstr to \p *this.
			 * @param cstr Value to append to \p *this.
			 * @return A reference to \p *this.
			 */
			String &operator+=(const char *cstr)
			{
				concat(cstr);
				return (*this);
			}

			/**
			 * @brief Append \p c to \p *this.
			 * @param c Value to append to \p *this.
			 * @return A reference to \p *this.
			 */
			String &operator+=(char c)
			{
				concat(c);
				return (*this);
			}

			/**
			 * @brief Append \p num to \p *this.
			 * @param num Value to append to \p *this.
			 * @return A reference to \p *this.
			 */
			String &operator+=(unsigned char num)
			{
				concat(num);
				return (*this);
			}

			/**
			 * @brief Append \p num to \p *this.
			 * @param num Value to append to \p *this.
			 * @return A reference to \p *this.
			 */
			String &operator+=(int num)
			{
				concat(num);
				return (*this);
			}

			/**
			 * @brief Append \p num to \p *this.
			 * @param num Value to append to \p *this.
			 * @return A reference to \p *this.
			 */
			String &operator+=(unsigned int num)
			{
				concat(num);
				return (*this);
			}

			/**
			 * @brief Append \p num to \p *this.
			 * @param num Value to append to \p *this.
			 * @return A reference to \p *this.
			 */
			String &operator+=(long num)
			{
				concat(num);
				return (*this);
			}

			/**
			 * @brief Append \p num to \p *this.
			 * @param num Value to append to \p *this.
			 * @return A reference to \p *this.
			 */
			String &operator+=(unsigned long num)
			{
				concat(num);
				return (*this);
			}

			/**
			 * @brief Append \p num to \p *this.
			 * @param num Value to append to \p *this.
			 * @return A reference to \p *this.
			 */
			String &operator+=(float num)
			{
				concat(num);
				return (*this);
			}

			/**
			 * @brief Append \p num to \p *this.
			 * @param num Value to append to \p *this.
			 * @return A reference to \p *this.
			 */
			String &operator+=(double num)
			{
				concat(num);
				return (*this);
			}

			friend StringSumHelper &operator+(const StringSumHelper &lhs, const String &rhs);

			friend StringSumHelper &operator+(const StringSumHelper &lhs, const char *cstr);

			friend StringSumHelper &operator+(const StringSumHelper &lhs, char c);

			friend StringSumHelper &operator+(const StringSumHelper &lhs, unsigned char num);

			friend StringSumHelper &operator+(const StringSumHelper &lhs, int num);

			friend StringSumHelper &operator+(const StringSumHelper &lhs, unsigned int num);

			friend StringSumHelper &operator+(const StringSumHelper &lhs, long num);

			friend StringSumHelper &operator+(const StringSumHelper &lhs, unsigned long num);

			friend StringSumHelper &operator+(const StringSumHelper &lhs, float num);

			friend StringSumHelper &operator+(const StringSumHelper &lhs, double num);

			// comparison (only works w/ Strings and "strings")
			operator StringIfHelperType() const
			{
				return buffer ? &String::StringIfHelper : nullptr;
			}

			/**
			 * @brief Compare \p *this against \p *s.
			 * @param s String to compare against.
			 * @retval <0 when the first character that doesn't match is smaller in \p *this than \p s.
			 * @retval 0 when both strings are equal.
			 * @retval >0 when the first character that doesn't match is bigger in \p *this than \p s.
			 */
			int compareTo(const String &s) const;

			/**
			 * @brief Check if \p *this equals \p s.
			 * @param s String to check against \p *this.
			 * @return True when equal, false otherwise.
			 */
			bool equals(const String &s) const;

			/**
			 * @brief Check if \p *this equals \p cstr.
			 * @param cstr String to check against \p *this.
			 * @return True when equal, false otherwise.
			 */
			bool equals(const char *cstr) const;

			/**
			 * @brief Check if \p *this equals \p rhs.
			 * @param rhs String to check against \p *this.
			 * @return True when equal, false otherwise.
			 */
			bool operator==(const String &rhs) const
			{
				return equals(rhs);
			}

			/**
			 * @brief Check if \p *this equals \p cstr.
			 * @param cstr String to check against \p *this.
			 * @return True when equal, false otherwise.
			 */
			bool operator==(const char *cstr) const
			{
				return equals(cstr);
			}

			/**
			 * @brief Check if \p *this equals \p rhs.
			 * @param rhs String to check against \p *this.
			 * @return True when equal, false otherwise.
			 */
			bool operator!=(const String &rhs) const
			{
				return !equals(rhs);
			}

			/**
			 * @brief Check if \p *this equals \p cstr.
			 * @param cstr String to check against \p *this.
			 * @return True when equal, false otherwise.
			 */
			bool operator!=(const char *cstr) const
			{
				return !equals(cstr);
			}

			/**
			 * @brief Check if \p *this equals \p rhs.
			 * @param rhs String to check against \p *this.
			 * @return True when equal, false otherwise.
			 */
			bool operator<(const String &rhs) const;

			/**
			 * @brief Check if \p *this equals \p rhs.
			 * @param rhs String to check against \p *this.
			 * @return True when equal, false otherwise.
			 */
			bool operator>(const String &rhs) const;

			/**
			 * @brief Check if \p *this equals \p rhs.
			 * @param rhs String to check against \p *this.
			 * @return True when equal, false otherwise.
			 */
			bool operator<=(const String &rhs) const;

			/**
			 * @brief Check if \p *this equals \p rhs.
			 * @param rhs String to check against \p *this.
			 * @return True when equal, false otherwise.
			 */
			bool operator>=(const String &rhs) const;

			/**
			 * @brief Check if \p *this equals \p s.
			 * @param s String to check against \p *this.
			 * @return True when equal, false otherwise.
			 */
			bool equalsIgnoreCase(const String &s) const;

			/**
			 * @brief Check of \p *this starts with \p prefix.
			 * @param prefix Prefix to test.
			 * @return True on success, false otherwise.
			 */
			bool startsWith(const String &prefix) const;

			/**
			 * @brief Check of \p *this starts with \p prefix.
			 * @param prefix Prefix to test.
			 * @param offset Number of characters to skip.
			 * @return True on success, false otherwise.
			 */
			bool startsWith(const String &prefix, unsigned int offset) const;

			/**
			 * @brief Check of \p *this ends with \p prefix.
			 * @param suffix Suffix to test.
			 * @return True on success, false otherwise.
			 */
			bool endsWith(const String &suffix) const;

			/**
			 * @brief Access the character at \p index.
			 * @param index Index to access.
			 * @return The character at \p index.
			 */
			char charAt(unsigned int index) const;

			/**
			 * @brief Update the character at \p index.
			 * @param index Index to access.
			 * @param c Character to set at \p index.
			 * @return The character at \p index.
			 */
			void setCharAt(unsigned int index, char c);

			/**
			 * @brief Access the character at \p index.
			 * @param index Index to access.
			 * @return The character at \p index.
			 */
			char operator[](unsigned int index) const;

			/**
			 * @brief Access the character at \p index.
			 * @param index Index to access.
			 * @return The character at \p index.
			 */
			char &operator[](unsigned int index);

			/**
			 * @brief Convert \p *this to a byte array.
			 * @param buf Target array.
			 * @param bufsize Size of \p buf.
			 * @param index Index to start at.
			 */
			void getBytes(unsigned char *buf, unsigned int bufsize, unsigned int index = 0) const;

			/**
			 * @brief Convert \p *this to a char array.
			 * @param buf Target array.
			 * @param bufsize Size of \p buf.
			 * @param index Index to start at.
			 */
			void toCharArray(char *buf, unsigned int bufsize, unsigned int index = 0) const
			{
				getBytes((unsigned char *) buf, bufsize, index);
			}

			/**
			 * @brief Get the C-style string representation of \p *this.
			 * @return The underlying C-style string.
			 */
			const char *c_str() const
			{
				return buffer;
			}

			/**
			 * @brief Get a reference to the first character of \p *this.
			 * @return An reference to the first character of \p *this.
			 */
			char *begin()
			{
				return buffer;
			}

			/**
			 * @brief Get a reference to the last character of \p *this.
			 * @return An reference to the last character of \p *this.
			 */
			char *end()
			{
				return buffer + length();
			}

			/**
			 * @brief Get a reference to the first character of \p *this.
			 * @return An reference to the first character of \p *this.
			 */
			const char *begin() const
			{
				return c_str();
			}

			/**
			 * @brief Get a reference to the last character of \p *this.
			 * @return An reference to the last character of \p *this.
			 */
			const char *end() const
			{
				return c_str() + length();
			}

			/**
			 * @brief Find the first occurance of \p ch.
			 * @param ch Value to search for.
			 * @return First occurance of \p ch or <0.
			 */
			long indexOf(char ch) const;

			/**
			 * @brief Find the first occurance of \p ch.
			 * @param ch Value to search for.
			 * @param fromIndex Start index.
			 * @return First occurance of \p ch or <0.
			 */
			long indexOf(char ch, unsigned int fromIndex) const;

			/**
			 * @brief Find the first occurance of \p ch.
			 * @param str Value to search for.
			 * @return First occurance of \p ch or <0.
			 */
			long indexOf(const String &str) const;

			/**
			 * @brief Find the first occurance of \p ch.
			 * @param str Value to search for.
			 * @param fromIndex Start index.
			 * @return First occurance of \p ch or <0.
			 */
			long indexOf(const String &str, unsigned int fromIndex) const;

			/**
			 * @brief Find the last occurance of \p ch.
			 * @param ch Value to search for.
			 * @return Last occurance of \p ch or <0.
			 */
			long lastIndexOf(char ch) const;

			/**
			 * @brief Find the last occurance of \p ch.
			 * @param ch Value to search for.
			 * @param fromIndex Start index.
			 * @return last occurance of \p ch or <0.
			 */
			long lastIndexOf(char ch, unsigned int fromIndex) const;

			/**
			 * @brief Find the last occurance of \p ch.
			 * @param str Value to search for.
			 * @return last occurance of \p ch or <0.
			 */
			long lastIndexOf(const String &str) const;

			/**
			 * @brief Find the last occurance of \p ch.
			 * @param str Value to search for.
			 * @param fromIndex Start index.
			 * @return last occurance of \p ch or <0.
			 */
			long lastIndexOf(const String &str, unsigned int fromIndex) const;

			/**
			 * @brief Get a substring of \p *this from a given index to the end.
			 * @param beginIndex Index to start.
			 * @return A substring of \p *this.
			 */
			String substring(unsigned int beginIndex) const
			{
				return substring(beginIndex, len);
			}

			/**
			 * @brief Get a substring of \p *this from a given index to another given index..
			 * @param beginIndex Index to start.
			 * @param endIndex Index to stop.
			 * @return A substring of \p *this.
			 */
			String substring(unsigned int beginIndex, unsigned int endIndex) const;

			/**
			 * @brief Replace a character.
			 * @param find Character to replace.
			 * @param replace New value.
			 */
			void replace(char find, char replace);

			/**
			 * @brief Replace a substring.
			 * @param find Substring to replace.
			 * @param replace New value.
			 */
			void replace(const String &find, const String &replace);

			/**
			 * @brief Remove the character at \p index.
			 * @param index Index to remove.
			 */
			void remove(unsigned int index);

			/**
			 * @brief Remove a number of characters starting at \p index.
			 * @param index Index to remove.
			 * @param count Number of characters to remove.
			 */
			void remove(unsigned int index, unsigned int count);

			/**
			 * @brief Convert \p *this to all lower case.
			 */
			void toLowerCase();

			/**
			 * @brief Convert \p *this to all upper case.
			 */
			void toUpperCase();

			/**
			 * @brief Trim \p *this.
			 */
			void trim();

			/**
			 * @brief Convert *this to an integer.
			 * @return Integer representation of \p *this.
			 */
			long toInt() const;

			/**
			 * @brief Convert *this to a float.
			 * @return Float representation of \p *this.
			 */
			float toFloat() const;

			/**
			 * @brief Convert *this to a double.
			 * @return Double representation of \p *this.
			 */
			double toDouble() const;

		protected:
			char *buffer; //!< Underlying C-style string.
			unsigned int capacity; //!< Capacity of \p buffer.
			unsigned int len; //!< Current string length.

		protected:
			void init();
			void invalidate();
			unsigned char changeBuffer(unsigned int maxStrLen);
			bool concat(const char *cstr, unsigned int length);

			// copy and move
			String &copy(const char *cstr, unsigned int length);
			String &copy(const uint8_t *cstr, unsigned int length);

#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__) || defined(WIN32)

			void move(String &rhs);

#endif
		};

		class StringSumHelper : public String {
		public:
			StringSumHelper(const String &s) : String(s)
			{
			}

			StringSumHelper(const char *p) : String(p)
			{
			}

			explicit StringSumHelper(char c) : String(c)
			{
			}

			explicit StringSumHelper(unsigned char num) : String(num)
			{
			}

			explicit StringSumHelper(int num) : String(num)
			{
			}

			explicit StringSumHelper(unsigned int num) : String(num)
			{
			}

			explicit StringSumHelper(long num) : String(num)
			{
			}

			explicit StringSumHelper(unsigned long num) : String(num)
			{
			}

			explicit StringSumHelper(float num) : String(num)
			{
			}

			explicit StringSumHelper(double num) : String(num)
			{
			}
		};
	}

	typedef stl::String String;
	typedef stl::StringSumHelper StringSumHelper;
}
#endif
