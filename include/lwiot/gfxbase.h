/*
 * Graphics base driver.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/types.h>
#include <lwiot/printer.h>

typedef struct { // Data stored PER GLYPH
	uint16_t bitmapOffset;     // Pointer into GFXfont->bitmap
	uint8_t  width, height;    // Bitmap dimensions in pixels
	uint8_t  xAdvance;         // Distance to advance cursor (x axis)
	int8_t   xOffset, yOffset; // Dist from cursor pos to UL corner
} GFXglyph;

typedef struct { // Data stored for FONT AS A WHOLE:
	uint8_t  *bitmap;      // Glyph bitmaps, concatenated
	GFXglyph *glyph;       // Glyph array
	uint8_t   first, last; // ASCII extents
	uint8_t   yAdvance;    // Newline distance (y axis)
} GFXfont;

namespace lwiot
{
	class GfxBase : public Printer {
	public:
		explicit GfxBase(int16_t w, int16_t h);
		virtual ~GfxBase() = default;

		virtual void drawPixel(int16_t x, int16_t y, uint16_t color) = 0;

		virtual void startWrite(void);
		virtual void writePixel(int16_t x, int16_t y, uint16_t color);
		virtual void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
		virtual void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
		virtual void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
		virtual void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
		virtual void endWrite(void);

		virtual void setRotation(uint8_t r);
		virtual void invert(bool i);

		virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
		virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
		virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
		virtual void fillScreen(uint16_t color);
		virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
		virtual void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

		void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
		void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
		void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
		void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
		void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
		void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
		void drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
		void fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
		void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color);
		void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color, uint16_t bg);
		void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
		void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg);
		void drawXBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color);
		void drawGrayscaleBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h);
		void drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h);
		void drawGrayscaleBitmap(int16_t x, int16_t y, const uint8_t bitmap[], const uint8_t mask[], int16_t w, int16_t h);
		void drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t *bitmap, uint8_t *mask, int16_t w, int16_t h);
		void drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h);
		void drawRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h);
		void drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], const uint8_t mask[], int16_t w, int16_t h);
		void drawRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, uint8_t *mask, int16_t w, int16_t h);
		void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
		void setCursor(int16_t x, int16_t y);
		void setTextColor(uint16_t c);
		void setTextColor(uint16_t c, uint16_t bg);
		void setTextSize(uint8_t s);
		void setTextWrap(bool w);
		void cp437(bool x=true);
		void setFont(const GFXfont *f = nullptr);
		void getTextBounds(char *string, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);

		virtual size_t write(uint8_t byte);
		int16_t height(void) const;
		int16_t width(void) const;
		uint8_t getRotation(void) const;

		int16_t getCursorX(void) const;
		int16_t getCursorY(void) const;

	protected:
		const int16_t WIDTH, HEIGHT;
		int16_t _width, _height, cursor_x, cursor_y;
		uint16_t textcolor, textbgcolor;
		uint8_t textsize, rotation;
		bool wrap, _cp437;
		GFXfont *gfxFont;

		void charBounds(char c, int16_t *x, int16_t *y, int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy);
	};
}
