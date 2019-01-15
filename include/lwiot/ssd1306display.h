/*
 * SSD1306 OLED display driver header.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/types.h>
#include <lwiot/kernel/lock.h>
#include <lwiot/i2cbus.h>
#include <lwiot/i2cmessage.h>
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
	class Ssd1306Display : public GfxBase {
	public:
		explicit Ssd1306Display(I2CBus& bus, uint8_t addr = Ssd1306Display::SlaveAddress);
		virtual ~Ssd1306Display() = default;

		void begin();

		void clear();
		virtual void invert(bool invert) override;
		void dim(bool dim);
		void display();

		void startScrollRight(uint8_t start, uint8_t stop);
		void startScrollLeft(uint8_t start, uint8_t stop);

		void startScrollDiagonalRight(uint8_t start, uint8_t stop);
		void startScrollDiagonalLeft(uint8_t start, uint8_t stop);

		void stopScroll();

		void drawPixel(int16_t x, int16_t y, uint16_t color) override;
		void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) override;
		void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) override;

	protected:
		void writeCommand(uint8_t cmd);

	private:
		I2CBus _bus;
		uint8_t _i2caddr;
		static constexpr uint8_t SlaveAddress = 0x3C;

		void inline rawDrawVerticalLine(int16_t x, int16_t y, int16_t h, uint16_t color);
		void inline rawDrawHorizontalLine(int16_t x, int16_t y, int16_t w, uint16_t color);
	};
}
