/*
 * SSD1306 OLED display driver header.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file ssd1306.h Display device driver.

#pragma once

#include <lwiot/types.h>
#include <lwiot/kernel/lock.h>
#include <lwiot/io/i2cbus.h>
#include <lwiot/io/i2cmessage.h>
#include <lwiot/gfxbase.h>

#define BLACK    0
#define WHITE    1
#define INVERSE  2

#ifdef CONFIG_SSD1306_128_64
#define SSD1306_128_64
#define SSD1306_LCDWIDTH  128
#define SSD1306_LCDHEIGHT  64
#endif

#ifdef CONFIG_SSD1306_128_32
#define SSD1306_128_32
#define SSD1306_LCDWIDTH  128
#define SSD1306_LCDHEIGHT  32
#endif

#ifdef CONFIG_SSD1306_96_16
#define SSD1306_96_16
#define SSD1306_LCDWIDTH  96
#define SSD1306_LCDHEIGHT 16
#endif

namespace lwiot
{
	/**
	 * @brief SSD1306 display driver.
	 * @ingroup device
	 */
	class Ssd1306Display : public GfxBase {
	public:
		/**
		 * @brief Construct a new SSD1306 device driver.
		 * @param bus I2C bus object.
		 * @param addr I2C slave address.
		 */
		explicit Ssd1306Display(I2CBus& bus, uint8_t addr = Ssd1306Display::SlaveAddress);

		/**
		 * @brief Destruct a SSD1306 object.
		 */
		~Ssd1306Display() override = default;

		/**
		 * @brief Start the SSD1306 drivers.
		 */
		void begin();

		/**
		 * @brief Clear the display.
		 */
		void clear();

		/**
		 * @brief Invert the display.
		 * @param invert Invert indicator.
		 */
		void invert(bool invert) override;

		/**
		 * @brief Dim the display.
		 * @param dim Dim indicator.
		 */
		void dim(bool dim);

		/**
		 * @brief Update the display.
		 */
		void display();

		/**
		 * @brief Scroll the display.
		 * @param start Start column.
		 * @param stop End column.
		 */
		void startScrollRight(uint8_t start, uint8_t stop);

		/**
		 * @brief Scroll the display.
		 * @param start Start column.
		 * @param stop End column.
		 */
		void startScrollLeft(uint8_t start, uint8_t stop);

		/**
		 * @brief Scroll diagonally to the right.
		 * @param start First row.
		 * @param stop Last row.
		 */
		void startScrollDiagonalRight(uint8_t start, uint8_t stop);

		/**
		 * @brief Scroll diagonally to the left.
		 * @param start First row.
		 * @param stop last row.
		 */
		void startScrollDiagonalLeft(uint8_t start, uint8_t stop);

		/**
		 * @brief Stop scrolling.
		 */
		void stopScroll();

		/**
		 * @brief Draw a pixel.
		 * @param x X-coordinate.
		 * @param y Y-coordinate.
		 * @param color Color value.
		 */
		void drawPixel(int16_t x, int16_t y, uint16_t color) override;

		/**
		 * @brief Draw a vertical line.
		 * @param x X-coordinate.
		 * @param y Y-coordinate.
		 * @param h Height.
		 * @param color Color value.
		 */
		void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) override;

		/**
		 * @brief Draw a horizontal line.
		 * @param x X-coordinate.
		 * @param y Y-coordinate.
		 * @param w Width.
		 * @param color Color value.
		 */
		void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) override;

	protected:
		/**
		 * @brief Write a command to the SSD1306 display.
		 * @param cmd Command to write.
		 */
		void writeCommand(uint8_t cmd);

	private:
		I2CBus _bus;
		uint8_t _i2caddr;
		static constexpr uint8_t SlaveAddress = 0x3C;

		void inline rawDrawVerticalLine(int16_t x, int16_t y, int16_t h, uint16_t color);
		void inline rawDrawHorizontalLine(int16_t x, int16_t y, int16_t w, uint16_t color);
	};
}
