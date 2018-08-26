/*
 * Graphics base driver.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/log.h>
#include <lwiot/printer.h>
#include <lwiot/gfxbase.h>

#ifdef AVR
#include <avr/pgmspace.h>
static const unsigned char default_font[] PROGMEM = {
#else
static const unsigned char default_font[] = {
#endif
		0x00, 0x00, 0x00, 0x00, 0x00,
		0x3E, 0x5B, 0x4F, 0x5B, 0x3E,
		0x3E, 0x6B, 0x4F, 0x6B, 0x3E,
		0x1C, 0x3E, 0x7C, 0x3E, 0x1C,
		0x18, 0x3C, 0x7E, 0x3C, 0x18,
		0x1C, 0x57, 0x7D, 0x57, 0x1C,
		0x1C, 0x5E, 0x7F, 0x5E, 0x1C,
		0x00, 0x18, 0x3C, 0x18, 0x00,
		0xFF, 0xE7, 0xC3, 0xE7, 0xFF,
		0x00, 0x18, 0x24, 0x18, 0x00,
		0xFF, 0xE7, 0xDB, 0xE7, 0xFF,
		0x30, 0x48, 0x3A, 0x06, 0x0E,
		0x26, 0x29, 0x79, 0x29, 0x26,
		0x40, 0x7F, 0x05, 0x05, 0x07,
		0x40, 0x7F, 0x05, 0x25, 0x3F,
		0x5A, 0x3C, 0xE7, 0x3C, 0x5A,
		0x7F, 0x3E, 0x1C, 0x1C, 0x08,
		0x08, 0x1C, 0x1C, 0x3E, 0x7F,
		0x14, 0x22, 0x7F, 0x22, 0x14,
		0x5F, 0x5F, 0x00, 0x5F, 0x5F,
		0x06, 0x09, 0x7F, 0x01, 0x7F,
		0x00, 0x66, 0x89, 0x95, 0x6A,
		0x60, 0x60, 0x60, 0x60, 0x60,
		0x94, 0xA2, 0xFF, 0xA2, 0x94,
		0x08, 0x04, 0x7E, 0x04, 0x08,
		0x10, 0x20, 0x7E, 0x20, 0x10,
		0x08, 0x08, 0x2A, 0x1C, 0x08,
		0x08, 0x1C, 0x2A, 0x08, 0x08,
		0x1E, 0x10, 0x10, 0x10, 0x10,
		0x0C, 0x1E, 0x0C, 0x1E, 0x0C,
		0x30, 0x38, 0x3E, 0x38, 0x30,
		0x06, 0x0E, 0x3E, 0x0E, 0x06,
		0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x5F, 0x00, 0x00,
		0x00, 0x07, 0x00, 0x07, 0x00,
		0x14, 0x7F, 0x14, 0x7F, 0x14,
		0x24, 0x2A, 0x7F, 0x2A, 0x12,
		0x23, 0x13, 0x08, 0x64, 0x62,
		0x36, 0x49, 0x56, 0x20, 0x50,
		0x00, 0x08, 0x07, 0x03, 0x00,
		0x00, 0x1C, 0x22, 0x41, 0x00,
		0x00, 0x41, 0x22, 0x1C, 0x00,
		0x2A, 0x1C, 0x7F, 0x1C, 0x2A,
		0x08, 0x08, 0x3E, 0x08, 0x08,
		0x00, 0x80, 0x70, 0x30, 0x00,
		0x08, 0x08, 0x08, 0x08, 0x08,
		0x00, 0x00, 0x60, 0x60, 0x00,
		0x20, 0x10, 0x08, 0x04, 0x02,
		0x3E, 0x51, 0x49, 0x45, 0x3E,
		0x00, 0x42, 0x7F, 0x40, 0x00,
		0x72, 0x49, 0x49, 0x49, 0x46,
		0x21, 0x41, 0x49, 0x4D, 0x33,
		0x18, 0x14, 0x12, 0x7F, 0x10,
		0x27, 0x45, 0x45, 0x45, 0x39,
		0x3C, 0x4A, 0x49, 0x49, 0x31,
		0x41, 0x21, 0x11, 0x09, 0x07,
		0x36, 0x49, 0x49, 0x49, 0x36,
		0x46, 0x49, 0x49, 0x29, 0x1E,
		0x00, 0x00, 0x14, 0x00, 0x00,
		0x00, 0x40, 0x34, 0x00, 0x00,
		0x00, 0x08, 0x14, 0x22, 0x41,
		0x14, 0x14, 0x14, 0x14, 0x14,
		0x00, 0x41, 0x22, 0x14, 0x08,
		0x02, 0x01, 0x59, 0x09, 0x06,
		0x3E, 0x41, 0x5D, 0x59, 0x4E,
		0x7C, 0x12, 0x11, 0x12, 0x7C,
		0x7F, 0x49, 0x49, 0x49, 0x36,
		0x3E, 0x41, 0x41, 0x41, 0x22,
		0x7F, 0x41, 0x41, 0x41, 0x3E,
		0x7F, 0x49, 0x49, 0x49, 0x41,
		0x7F, 0x09, 0x09, 0x09, 0x01,
		0x3E, 0x41, 0x41, 0x51, 0x73,
		0x7F, 0x08, 0x08, 0x08, 0x7F,
		0x00, 0x41, 0x7F, 0x41, 0x00,
		0x20, 0x40, 0x41, 0x3F, 0x01,
		0x7F, 0x08, 0x14, 0x22, 0x41,
		0x7F, 0x40, 0x40, 0x40, 0x40,
		0x7F, 0x02, 0x1C, 0x02, 0x7F,
		0x7F, 0x04, 0x08, 0x10, 0x7F,
		0x3E, 0x41, 0x41, 0x41, 0x3E,
		0x7F, 0x09, 0x09, 0x09, 0x06,
		0x3E, 0x41, 0x51, 0x21, 0x5E,
		0x7F, 0x09, 0x19, 0x29, 0x46,
		0x26, 0x49, 0x49, 0x49, 0x32,
		0x03, 0x01, 0x7F, 0x01, 0x03,
		0x3F, 0x40, 0x40, 0x40, 0x3F,
		0x1F, 0x20, 0x40, 0x20, 0x1F,
		0x3F, 0x40, 0x38, 0x40, 0x3F,
		0x63, 0x14, 0x08, 0x14, 0x63,
		0x03, 0x04, 0x78, 0x04, 0x03,
		0x61, 0x59, 0x49, 0x4D, 0x43,
		0x00, 0x7F, 0x41, 0x41, 0x41,
		0x02, 0x04, 0x08, 0x10, 0x20,
		0x00, 0x41, 0x41, 0x41, 0x7F,
		0x04, 0x02, 0x01, 0x02, 0x04,
		0x40, 0x40, 0x40, 0x40, 0x40,
		0x00, 0x03, 0x07, 0x08, 0x00,
		0x20, 0x54, 0x54, 0x78, 0x40,
		0x7F, 0x28, 0x44, 0x44, 0x38,
		0x38, 0x44, 0x44, 0x44, 0x28,
		0x38, 0x44, 0x44, 0x28, 0x7F,
		0x38, 0x54, 0x54, 0x54, 0x18,
		0x00, 0x08, 0x7E, 0x09, 0x02,
		0x18, 0xA4, 0xA4, 0x9C, 0x78,
		0x7F, 0x08, 0x04, 0x04, 0x78,
		0x00, 0x44, 0x7D, 0x40, 0x00,
		0x20, 0x40, 0x40, 0x3D, 0x00,
		0x7F, 0x10, 0x28, 0x44, 0x00,
		0x00, 0x41, 0x7F, 0x40, 0x00,
		0x7C, 0x04, 0x78, 0x04, 0x78,
		0x7C, 0x08, 0x04, 0x04, 0x78,
		0x38, 0x44, 0x44, 0x44, 0x38,
		0xFC, 0x18, 0x24, 0x24, 0x18,
		0x18, 0x24, 0x24, 0x18, 0xFC,
		0x7C, 0x08, 0x04, 0x04, 0x08,
		0x48, 0x54, 0x54, 0x54, 0x24,
		0x04, 0x04, 0x3F, 0x44, 0x24,
		0x3C, 0x40, 0x40, 0x20, 0x7C,
		0x1C, 0x20, 0x40, 0x20, 0x1C,
		0x3C, 0x40, 0x30, 0x40, 0x3C,
		0x44, 0x28, 0x10, 0x28, 0x44,
		0x4C, 0x90, 0x90, 0x90, 0x7C,
		0x44, 0x64, 0x54, 0x4C, 0x44,
		0x00, 0x08, 0x36, 0x41, 0x00,
		0x00, 0x00, 0x77, 0x00, 0x00,
		0x00, 0x41, 0x36, 0x08, 0x00,
		0x02, 0x01, 0x02, 0x04, 0x02,
		0x3C, 0x26, 0x23, 0x26, 0x3C,
		0x1E, 0xA1, 0xA1, 0x61, 0x12,
		0x3A, 0x40, 0x40, 0x20, 0x7A,
		0x38, 0x54, 0x54, 0x55, 0x59,
		0x21, 0x55, 0x55, 0x79, 0x41,
		0x22, 0x54, 0x54, 0x78, 0x42, // a-umlaut
		0x21, 0x55, 0x54, 0x78, 0x40,
		0x20, 0x54, 0x55, 0x79, 0x40,
		0x0C, 0x1E, 0x52, 0x72, 0x12,
		0x39, 0x55, 0x55, 0x55, 0x59,
		0x39, 0x54, 0x54, 0x54, 0x59,
		0x39, 0x55, 0x54, 0x54, 0x58,
		0x00, 0x00, 0x45, 0x7C, 0x41,
		0x00, 0x02, 0x45, 0x7D, 0x42,
		0x00, 0x01, 0x45, 0x7C, 0x40,
		0x7D, 0x12, 0x11, 0x12, 0x7D, // A-umlaut
		0xF0, 0x28, 0x25, 0x28, 0xF0,
		0x7C, 0x54, 0x55, 0x45, 0x00,
		0x20, 0x54, 0x54, 0x7C, 0x54,
		0x7C, 0x0A, 0x09, 0x7F, 0x49,
		0x32, 0x49, 0x49, 0x49, 0x32,
		0x3A, 0x44, 0x44, 0x44, 0x3A, // o-umlaut
		0x32, 0x4A, 0x48, 0x48, 0x30,
		0x3A, 0x41, 0x41, 0x21, 0x7A,
		0x3A, 0x42, 0x40, 0x20, 0x78,
		0x00, 0x9D, 0xA0, 0xA0, 0x7D,
		0x3D, 0x42, 0x42, 0x42, 0x3D, // O-umlaut
		0x3D, 0x40, 0x40, 0x40, 0x3D,
		0x3C, 0x24, 0xFF, 0x24, 0x24,
		0x48, 0x7E, 0x49, 0x43, 0x66,
		0x2B, 0x2F, 0xFC, 0x2F, 0x2B,
		0xFF, 0x09, 0x29, 0xF6, 0x20,
		0xC0, 0x88, 0x7E, 0x09, 0x03,
		0x20, 0x54, 0x54, 0x79, 0x41,
		0x00, 0x00, 0x44, 0x7D, 0x41,
		0x30, 0x48, 0x48, 0x4A, 0x32,
		0x38, 0x40, 0x40, 0x22, 0x7A,
		0x00, 0x7A, 0x0A, 0x0A, 0x72,
		0x7D, 0x0D, 0x19, 0x31, 0x7D,
		0x26, 0x29, 0x29, 0x2F, 0x28,
		0x26, 0x29, 0x29, 0x29, 0x26,
		0x30, 0x48, 0x4D, 0x40, 0x20,
		0x38, 0x08, 0x08, 0x08, 0x08,
		0x08, 0x08, 0x08, 0x08, 0x38,
		0x2F, 0x10, 0xC8, 0xAC, 0xBA,
		0x2F, 0x10, 0x28, 0x34, 0xFA,
		0x00, 0x00, 0x7B, 0x00, 0x00,
		0x08, 0x14, 0x2A, 0x14, 0x22,
		0x22, 0x14, 0x2A, 0x14, 0x08,
		0x55, 0x00, 0x55, 0x00, 0x55, // #176 (25% block) missing in old code
		0xAA, 0x55, 0xAA, 0x55, 0xAA, // 50% block
		0xFF, 0x55, 0xFF, 0x55, 0xFF, // 75% block
		0x00, 0x00, 0x00, 0xFF, 0x00,
		0x10, 0x10, 0x10, 0xFF, 0x00,
		0x14, 0x14, 0x14, 0xFF, 0x00,
		0x10, 0x10, 0xFF, 0x00, 0xFF,
		0x10, 0x10, 0xF0, 0x10, 0xF0,
		0x14, 0x14, 0x14, 0xFC, 0x00,
		0x14, 0x14, 0xF7, 0x00, 0xFF,
		0x00, 0x00, 0xFF, 0x00, 0xFF,
		0x14, 0x14, 0xF4, 0x04, 0xFC,
		0x14, 0x14, 0x17, 0x10, 0x1F,
		0x10, 0x10, 0x1F, 0x10, 0x1F,
		0x14, 0x14, 0x14, 0x1F, 0x00,
		0x10, 0x10, 0x10, 0xF0, 0x00,
		0x00, 0x00, 0x00, 0x1F, 0x10,
		0x10, 0x10, 0x10, 0x1F, 0x10,
		0x10, 0x10, 0x10, 0xF0, 0x10,
		0x00, 0x00, 0x00, 0xFF, 0x10,
		0x10, 0x10, 0x10, 0x10, 0x10,
		0x10, 0x10, 0x10, 0xFF, 0x10,
		0x00, 0x00, 0x00, 0xFF, 0x14,
		0x00, 0x00, 0xFF, 0x00, 0xFF,
		0x00, 0x00, 0x1F, 0x10, 0x17,
		0x00, 0x00, 0xFC, 0x04, 0xF4,
		0x14, 0x14, 0x17, 0x10, 0x17,
		0x14, 0x14, 0xF4, 0x04, 0xF4,
		0x00, 0x00, 0xFF, 0x00, 0xF7,
		0x14, 0x14, 0x14, 0x14, 0x14,
		0x14, 0x14, 0xF7, 0x00, 0xF7,
		0x14, 0x14, 0x14, 0x17, 0x14,
		0x10, 0x10, 0x1F, 0x10, 0x1F,
		0x14, 0x14, 0x14, 0xF4, 0x14,
		0x10, 0x10, 0xF0, 0x10, 0xF0,
		0x00, 0x00, 0x1F, 0x10, 0x1F,
		0x00, 0x00, 0x00, 0x1F, 0x14,
		0x00, 0x00, 0x00, 0xFC, 0x14,
		0x00, 0x00, 0xF0, 0x10, 0xF0,
		0x10, 0x10, 0xFF, 0x10, 0xFF,
		0x14, 0x14, 0x14, 0xFF, 0x14,
		0x10, 0x10, 0x10, 0x1F, 0x00,
		0x00, 0x00, 0x00, 0xF0, 0x10,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
		0xFF, 0xFF, 0xFF, 0x00, 0x00,
		0x00, 0x00, 0x00, 0xFF, 0xFF,
		0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
		0x38, 0x44, 0x44, 0x38, 0x44,
		0xFC, 0x4A, 0x4A, 0x4A, 0x34, // sharp-s or beta
		0x7E, 0x02, 0x02, 0x06, 0x06,
		0x02, 0x7E, 0x02, 0x7E, 0x02,
		0x63, 0x55, 0x49, 0x41, 0x63,
		0x38, 0x44, 0x44, 0x3C, 0x04,
		0x40, 0x7E, 0x20, 0x1E, 0x20,
		0x06, 0x02, 0x7E, 0x02, 0x02,
		0x99, 0xA5, 0xE7, 0xA5, 0x99,
		0x1C, 0x2A, 0x49, 0x2A, 0x1C,
		0x4C, 0x72, 0x01, 0x72, 0x4C,
		0x30, 0x4A, 0x4D, 0x4D, 0x30,
		0x30, 0x48, 0x78, 0x48, 0x30,
		0xBC, 0x62, 0x5A, 0x46, 0x3D,
		0x3E, 0x49, 0x49, 0x49, 0x00,
		0x7E, 0x01, 0x01, 0x01, 0x7E,
		0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
		0x44, 0x44, 0x5F, 0x44, 0x44,
		0x40, 0x51, 0x4A, 0x44, 0x40,
		0x40, 0x44, 0x4A, 0x51, 0x40,
		0x00, 0x00, 0xFF, 0x01, 0x03,
		0xE0, 0x80, 0xFF, 0x00, 0x00,
		0x08, 0x08, 0x6B, 0x6B, 0x08,
		0x36, 0x12, 0x36, 0x24, 0x36,
		0x06, 0x0F, 0x09, 0x0F, 0x06,
		0x00, 0x00, 0x18, 0x18, 0x00,
		0x00, 0x00, 0x10, 0x10, 0x00,
		0x30, 0x40, 0xFF, 0x01, 0x01,
		0x00, 0x1F, 0x01, 0x01, 0x1E,
		0x00, 0x19, 0x1D, 0x17, 0x12,
		0x00, 0x3C, 0x3C, 0x3C, 0x3C,
		0x00, 0x00, 0x00, 0x00, 0x00  // #255 NBSP
};

#ifndef pgm_read_byte
#define pgm_read_byte(addr) (*(unsigned char *)(addr))
#endif
#ifndef pgm_read_word
#define pgm_read_word(addr) (*(unsigned short *)(addr))
#endif
#ifndef pgm_read_dword
static inline unsigned long pgm_read_dword(void* addr)
{
	unsigned long *data = (unsigned long*)addr;
	return *data;
}
#endif

#if !defined(__INT_MAX__) || (__INT_MAX__ > 0xFFFF)
#define pgm_read_pointer(addr, type) ((type *)pgm_read_dword((void*)addr))
#else
#define pgm_read_pointer(addr, type) ((type *)pgm_read_word((size_t)addr))
#endif

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif


namespace lwiot
{
	GfxBase::GfxBase(int16_t w, int16_t h) :
			WIDTH(w), HEIGHT(h)
	{
		_width = WIDTH;
		_height = HEIGHT;
		rotation = 0;
		cursor_y = cursor_x = 0;
		textsize = 1;
		textcolor = textbgcolor = 0xFFFF;
		wrap = true;
		_cp437 = false;
		gfxFont = NULL;
	}

	void GfxBase::writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
	{
		int16_t steep = abs(y1 - y0) > abs(x1 - x0);
		if(steep) {
			_swap_int16_t(x0, y0);
			_swap_int16_t(x1, y1);
		}

		if(x0 > x1) {
			_swap_int16_t(x0, x1);
			_swap_int16_t(y0, y1);
		}

		int16_t dx, dy;
		dx = x1 - x0;
		dy = abs(y1 - y0);

		int16_t err = dx / 2;
		int16_t ystep;

		if(y0 < y1) {
			ystep = 1;
		} else {
			ystep = -1;
		}

		for(; x0 <= x1; x0++) {
			if(steep) {
				writePixel(y0, x0, color);
			} else {
				writePixel(x0, y0, color);
			}
			err -= dy;
			if(err < 0) {
				y0 += ystep;
				err += dx;
			}
		}
	}

	void GfxBase::startWrite()
	{
	}

	void GfxBase::writePixel(int16_t x, int16_t y, uint16_t color)
	{
		drawPixel(x, y, color);
	}

	void GfxBase::writeFastVLine(int16_t x, int16_t y,
	                             int16_t h, uint16_t color)
	{
		drawFastVLine(x, y, h, color);
	}

	void GfxBase::writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
	{
		drawFastHLine(x, y, w, color);
	}

	void GfxBase::writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h,
	                            uint16_t color)
	{
		fillRect(x, y, w, h, color);
	}

	void GfxBase::endWrite()
	{
	}

	void GfxBase::drawFastVLine(int16_t x, int16_t y,
	                            int16_t h, uint16_t color)
	{
		startWrite();
		writeLine(x, y, x, y + h - 1, color);
		endWrite();
	}

	void GfxBase::drawFastHLine(int16_t x, int16_t y,
	                            int16_t w, uint16_t color)
	{
		startWrite();
		writeLine(x, y, x + w - 1, y, color);
		endWrite();
	}

	void GfxBase::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
	                       uint16_t color)
	{
		startWrite();
		for(int16_t i = x; i < x + w; i++) {
			writeFastVLine(i, y, h, color);
		}
		endWrite();
	}

	void GfxBase::fillScreen(uint16_t color)
	{
		fillRect(0, 0, _width, _height, color);
	}

	void GfxBase::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
	                       uint16_t color)
	{
		if(x0 == x1) {
			if(y0 > y1)
				_swap_int16_t(y0, y1);
			drawFastVLine(x0, y0, y1 - y0 + 1, color);
		} else if(y0 == y1) {
			if(x0 > x1)
				_swap_int16_t(x0, x1);
			drawFastHLine(x0, y0, x1 - x0 + 1, color);
		} else {
			startWrite();
			writeLine(x0, y0, x1, y1, color);
			endWrite();
		}
	}

// Draw a circle outline
	void GfxBase::drawCircle(int16_t x0, int16_t y0, int16_t r,
	                         uint16_t color)
	{
		int16_t f = 1 - r;
		int16_t ddF_x = 1;
		int16_t ddF_y = -2 * r;
		int16_t x = 0;
		int16_t y = r;

		startWrite();
		writePixel(x0, y0 + r, color);
		writePixel(x0, y0 - r, color);
		writePixel(x0 + r, y0, color);
		writePixel(x0 - r, y0, color);

		while(x < y) {
			if(f >= 0) {
				y--;
				ddF_y += 2;
				f += ddF_y;
			}
			x++;
			ddF_x += 2;
			f += ddF_x;

			writePixel(x0 + x, y0 + y, color);
			writePixel(x0 - x, y0 + y, color);
			writePixel(x0 + x, y0 - y, color);
			writePixel(x0 - x, y0 - y, color);
			writePixel(x0 + y, y0 + x, color);
			writePixel(x0 - y, y0 + x, color);
			writePixel(x0 + y, y0 - x, color);
			writePixel(x0 - y, y0 - x, color);
		}
		endWrite();
	}

	void GfxBase::drawCircleHelper(int16_t x0, int16_t y0,
	                               int16_t r, uint8_t cornername, uint16_t color)
	{
		int16_t f = 1 - r;
		int16_t ddF_x = 1;
		int16_t ddF_y = -2 * r;
		int16_t x = 0;
		int16_t y = r;

		while(x < y) {
			if(f >= 0) {
				y--;
				ddF_y += 2;
				f += ddF_y;
			}
			x++;
			ddF_x += 2;
			f += ddF_x;
			if(cornername & 0x4) {
				writePixel(x0 + x, y0 + y, color);
				writePixel(x0 + y, y0 + x, color);
			}
			if(cornername & 0x2) {
				writePixel(x0 + x, y0 - y, color);
				writePixel(x0 + y, y0 - x, color);
			}
			if(cornername & 0x8) {
				writePixel(x0 - y, y0 + x, color);
				writePixel(x0 - x, y0 + y, color);
			}
			if(cornername & 0x1) {
				writePixel(x0 - y, y0 - x, color);
				writePixel(x0 - x, y0 - y, color);
			}
		}
	}

	void GfxBase::fillCircle(int16_t x0, int16_t y0, int16_t r,
	                         uint16_t color)
	{
		startWrite();
		writeFastVLine(x0, y0 - r, 2 * r + 1, color);
		fillCircleHelper(x0, y0, r, 3, 0, color);
		endWrite();
	}

// Used to do circles and roundrects
	void GfxBase::fillCircleHelper(int16_t x0, int16_t y0, int16_t r,
	                               uint8_t cornername, int16_t delta, uint16_t color)
	{

		int16_t f = 1 - r;
		int16_t ddF_x = 1;
		int16_t ddF_y = -2 * r;
		int16_t x = 0;
		int16_t y = r;

		while(x < y) {
			if(f >= 0) {
				y--;
				ddF_y += 2;
				f += ddF_y;
			}
			x++;
			ddF_x += 2;
			f += ddF_x;

			if(cornername & 0x1) {
				writeFastVLine(x0 + x, y0 - y, 2 * y + 1 + delta, color);
				writeFastVLine(x0 + y, y0 - x, 2 * x + 1 + delta, color);
			}
			if(cornername & 0x2) {
				writeFastVLine(x0 - x, y0 - y, 2 * y + 1 + delta, color);
				writeFastVLine(x0 - y, y0 - x, 2 * x + 1 + delta, color);
			}
		}
	}

// Draw a rectangle
	void GfxBase::drawRect(int16_t x, int16_t y, int16_t w, int16_t h,
	                       uint16_t color)
	{
		startWrite();
		writeFastHLine(x, y, w, color);
		writeFastHLine(x, y + h - 1, w, color);
		writeFastVLine(x, y, h, color);
		writeFastVLine(x + w - 1, y, h, color);
		endWrite();
	}

// Draw a rounded rectangle
	void GfxBase::drawRoundRect(int16_t x, int16_t y, int16_t w,
	                            int16_t h, int16_t r, uint16_t color)
	{
		// smarter version
		startWrite();
		writeFastHLine(x + r, y, w - 2 * r, color); // Top
		writeFastHLine(x + r, y + h - 1, w - 2 * r, color); // Bottom
		writeFastVLine(x, y + r, h - 2 * r, color); // Left
		writeFastVLine(x + w - 1, y + r, h - 2 * r, color); // Right
		// draw four corners
		drawCircleHelper(x + r, y + r, r, 1, color);
		drawCircleHelper(x + w - r - 1, y + r, r, 2, color);
		drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
		drawCircleHelper(x + r, y + h - r - 1, r, 8, color);
		endWrite();
	}

// Fill a rounded rectangle
	void GfxBase::fillRoundRect(int16_t x, int16_t y, int16_t w,
	                            int16_t h, int16_t r, uint16_t color)
	{
		// smarter version
		startWrite();
		writeFillRect(x + r, y, w - 2 * r, h, color);

		// draw four corners
		fillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
		fillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 1, color);
		endWrite();
	}

// Draw a triangle
	void GfxBase::drawTriangle(int16_t x0, int16_t y0,
	                           int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
	{
		drawLine(x0, y0, x1, y1, color);
		drawLine(x1, y1, x2, y2, color);
		drawLine(x2, y2, x0, y0, color);
	}

// Fill a triangle
	void GfxBase::fillTriangle(int16_t x0, int16_t y0,
	                           int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
	{

		int16_t a, b, y, last;

		// Sort coordinates by Y order (y2 >= y1 >= y0)
		if(y0 > y1) {
			_swap_int16_t(y0, y1);
			_swap_int16_t(x0, x1);
		}
		if(y1 > y2) {
			_swap_int16_t(y2, y1);
			_swap_int16_t(x2, x1);
		}
		if(y0 > y1) {
			_swap_int16_t(y0, y1);
			_swap_int16_t(x0, x1);
		}

		startWrite();
		if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
			a = b = x0;
			if(x1 < a)
				a = x1;
			else if(x1 > b)
				b = x1;
			if(x2 < a)
				a = x2;
			else if(x2 > b)
				b = x2;
			writeFastHLine(a, y0, b - a + 1, color);
			endWrite();
			return;
		}

		int16_t
				dx01 = x1 - x0,
				dy01 = y1 - y0,
				dx02 = x2 - x0,
				dy02 = y2 - y0,
				dx12 = x2 - x1,
				dy12 = y2 - y1;
		int32_t
				sa = 0,
				sb = 0;

		// For upper part of triangle, find scanline crossings for segments
		// 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
		// is included here (and second loop will be skipped, avoiding a /0
		// error there), otherwise scanline y1 is skipped here and handled
		// in the second loop...which also avoids a /0 error here if y0=y1
		// (flat-topped triangle).
		if(y1 == y2)
			last = y1;   // Include y1 scanline
		else
			last = y1 - 1; // Skip it

		for(y = y0; y <= last; y++) {
			a = x0 + sa / dy01;
			b = x0 + sb / dy02;
			sa += dx01;
			sb += dx02;
			/* longhand:
			a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
			b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
			*/
			if(a > b)
				_swap_int16_t(a, b);
			writeFastHLine(a, y, b - a + 1, color);
		}

		// For lower part of triangle, find scanline crossings for segments
		// 0-2 and 1-2.  This loop is skipped if y1=y2.
		sa = dx12 * (y - y1);
		sb = dx02 * (y - y0);
		for(; y <= y2; y++) {
			a = x1 + sa / dy12;
			b = x0 + sb / dy02;
			sa += dx12;
			sb += dx02;
			/* longhand:
			a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
			b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
			*/
			if(a > b)
				_swap_int16_t(a, b);
			writeFastHLine(a, y, b - a + 1, color);
		}
		endWrite();
	}

// BITMAP / XBITMAP / GRAYSCALE / RGB BITMAP FUNCTIONS ---------------------

// Draw a PROGMEM-resident 1-bit image at the specified (x,y) position,
// using the specified foreground color (unset bits are transparent).
	void GfxBase::drawBitmap(int16_t x, int16_t y,
	                         const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color)
	{

		int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
		uint8_t byte = 0;

		startWrite();
		for(int16_t j = 0; j < h; j++, y++) {
			for(int16_t i = 0; i < w; i++) {
				if(i & 7)
					byte <<= 1;
				else
					byte = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);
				if(byte & 0x80)
					writePixel(x + i, y, color);
			}
		}
		endWrite();
	}

// Draw a PROGMEM-resident 1-bit image at the specified (x,y) position,
// using the specified foreground (for set bits) and background (unset
// bits) colors.
	void GfxBase::drawBitmap(int16_t x, int16_t y,
	                         const uint8_t bitmap[], int16_t w, int16_t h,
	                         uint16_t color, uint16_t bg)
	{

		int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
		uint8_t byte = 0;

		startWrite();
		for(int16_t j = 0; j < h; j++, y++) {
			for(int16_t i = 0; i < w; i++) {
				if(i & 7)
					byte <<= 1;
				else
					byte = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);
				writePixel(x + i, y, (byte & 0x80) ? color : bg);
			}
		}
		endWrite();
	}

// Draw a RAM-resident 1-bit image at the specified (x,y) position,
// using the specified foreground color (unset bits are transparent).
	void GfxBase::drawBitmap(int16_t x, int16_t y,
	                         uint8_t *bitmap, int16_t w, int16_t h, uint16_t color)
	{

		int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
		uint8_t byte = 0;

		startWrite();
		for(int16_t j = 0; j < h; j++, y++) {
			for(int16_t i = 0; i < w; i++) {
				if(i & 7)
					byte <<= 1;
				else
					byte = bitmap[j * byteWidth + i / 8];
				if(byte & 0x80)
					writePixel(x + i, y, color);
			}
		}
		endWrite();
	}

// Draw a RAM-resident 1-bit image at the specified (x,y) position,
// using the specified foreground (for set bits) and background (unset
// bits) colors.
	void GfxBase::drawBitmap(int16_t x, int16_t y,
	                         uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg)
	{

		int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
		uint8_t byte = 0;

		startWrite();
		for(int16_t j = 0; j < h; j++, y++) {
			for(int16_t i = 0; i < w; i++) {
				if(i & 7)
					byte <<= 1;
				else
					byte = bitmap[j * byteWidth + i / 8];
				writePixel(x + i, y, (byte & 0x80) ? color : bg);
			}
		}
		endWrite();
	}

// Draw PROGMEM-resident XBitMap Files (*.xbm), exported from GIMP,
// Usage: Export from GIMP to *.xbm, rename *.xbm to *.c and open in editor.
// C Array can be directly used with this function.
// There is no RAM-resident version of this function; if generating bitmaps
// in RAM, use the format defined by drawBitmap() and call that instead.
	void GfxBase::drawXBitmap(int16_t x, int16_t y,
	                          const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color)
	{

		int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
		uint8_t byte = 0;

		startWrite();
		for(int16_t j = 0; j < h; j++, y++) {
			for(int16_t i = 0; i < w; i++) {
				if(i & 7)
					byte >>= 1;
				else
					byte = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);
				// Nearly identical to drawBitmap(), only the bit order
				// is reversed here (left-to-right = LSB to MSB):
				if(byte & 0x01)
					writePixel(x + i, y, color);
			}
		}
		endWrite();
	}

// Draw a PROGMEM-resident 8-bit image (grayscale) at the specified (x,y)
// pos.  Specifically for 8-bit display devices such as IS31FL3731;
// no color reduction/expansion is performed.
	void GfxBase::drawGrayscaleBitmap(int16_t x, int16_t y,
	                                  const uint8_t bitmap[], int16_t w, int16_t h)
	{
		startWrite();
		for(int16_t j = 0; j < h; j++, y++) {
			for(int16_t i = 0; i < w; i++) {
				writePixel(x + i, y, (uint8_t) pgm_read_byte(&bitmap[j * w + i]));
			}
		}
		endWrite();
	}

// Draw a RAM-resident 8-bit image (grayscale) at the specified (x,y)
// pos.  Specifically for 8-bit display devices such as IS31FL3731;
// no color reduction/expansion is performed.
	void GfxBase::drawGrayscaleBitmap(int16_t x, int16_t y,
	                                  uint8_t *bitmap, int16_t w, int16_t h)
	{
		startWrite();
		for(int16_t j = 0; j < h; j++, y++) {
			for(int16_t i = 0; i < w; i++) {
				writePixel(x + i, y, bitmap[j * w + i]);
			}
		}
		endWrite();
	}

// Draw a PROGMEM-resident 8-bit image (grayscale) with a 1-bit mask
// (set bits = opaque, unset bits = clear) at the specified (x,y) position.
// BOTH buffers (grayscale and mask) must be PROGMEM-resident.
// Specifically for 8-bit display devices such as IS31FL3731;
// no color reduction/expansion is performed.
	void GfxBase::drawGrayscaleBitmap(int16_t x, int16_t y,
	                                  const uint8_t bitmap[], const uint8_t mask[],
	                                  int16_t w, int16_t h)
	{
		int16_t bw = (w + 7) / 8; // Bitmask scanline pad = whole byte
		uint8_t byte = 0;
		startWrite();
		for(int16_t j = 0; j < h; j++, y++) {
			for(int16_t i = 0; i < w; i++) {
				if(i & 7)
					byte <<= 1;
				else
					byte = pgm_read_byte(&mask[j * bw + i / 8]);
				if(byte & 0x80) {
					writePixel(x + i, y, (uint8_t) pgm_read_byte(&bitmap[j * w + i]));
				}
			}
		}
		endWrite();
	}

// Draw a RAM-resident 8-bit image (grayscale) with a 1-bit mask
// (set bits = opaque, unset bits = clear) at the specified (x,y) pos.
// BOTH buffers (grayscale and mask) must be RAM-resident, no mix-and-
// match.  Specifically for 8-bit display devices such as IS31FL3731;
// no color reduction/expansion is performed.
	void GfxBase::drawGrayscaleBitmap(int16_t x, int16_t y,
	                                  uint8_t *bitmap, uint8_t *mask, int16_t w, int16_t h)
	{
		int16_t bw = (w + 7) / 8; // Bitmask scanline pad = whole byte
		uint8_t byte = 0;
		startWrite();
		for(int16_t j = 0; j < h; j++, y++) {
			for(int16_t i = 0; i < w; i++) {
				if(i & 7)
					byte <<= 1;
				else
					byte = mask[j * bw + i / 8];
				if(byte & 0x80) {
					writePixel(x + i, y, bitmap[j * w + i]);
				}
			}
		}
		endWrite();
	}

// Draw a PROGMEM-resident 16-bit image (RGB 5/6/5) at the specified (x,y)
// position.  For 16-bit display devices; no color reduction performed.
	void GfxBase::drawRGBBitmap(int16_t x, int16_t y,
	                            const uint16_t bitmap[], int16_t w, int16_t h)
	{
		startWrite();
		for(int16_t j = 0; j < h; j++, y++) {
			for(int16_t i = 0; i < w; i++) {
				writePixel(x + i, y, pgm_read_word(&bitmap[j * w + i]));
			}
		}
		endWrite();
	}

// Draw a RAM-resident 16-bit image (RGB 5/6/5) at the specified (x,y)
// position.  For 16-bit display devices; no color reduction performed.
	void GfxBase::drawRGBBitmap(int16_t x, int16_t y,
	                            uint16_t *bitmap, int16_t w, int16_t h)
	{
		startWrite();
		for(int16_t j = 0; j < h; j++, y++) {
			for(int16_t i = 0; i < w; i++) {
				writePixel(x + i, y, bitmap[j * w + i]);
			}
		}
		endWrite();
	}

// Draw a PROGMEM-resident 16-bit image (RGB 5/6/5) with a 1-bit mask
// (set bits = opaque, unset bits = clear) at the specified (x,y) position.
// BOTH buffers (color and mask) must be PROGMEM-resident.
// For 16-bit display devices; no color reduction performed.
	void GfxBase::drawRGBBitmap(int16_t x, int16_t y,
	                            const uint16_t bitmap[], const uint8_t mask[],
	                            int16_t w, int16_t h)
	{
		int16_t bw = (w + 7) / 8; // Bitmask scanline pad = whole byte
		uint8_t byte = 0;
		startWrite();
		for(int16_t j = 0; j < h; j++, y++) {
			for(int16_t i = 0; i < w; i++) {
				if(i & 7)
					byte <<= 1;
				else
					byte = pgm_read_byte(&mask[j * bw + i / 8]);
				if(byte & 0x80) {
					writePixel(x + i, y, pgm_read_word(&bitmap[j * w + i]));
				}
			}
		}
		endWrite();
	}

// Draw a RAM-resident 16-bit image (RGB 5/6/5) with a 1-bit mask
// (set bits = opaque, unset bits = clear) at the specified (x,y) pos.
// BOTH buffers (color and mask) must be RAM-resident, no mix-and-match.
// For 16-bit display devices; no color reduction performed.
	void GfxBase::drawRGBBitmap(int16_t x, int16_t y,
	                            uint16_t *bitmap, uint8_t *mask, int16_t w, int16_t h)
	{
		int16_t bw = (w + 7) / 8; // Bitmask scanline pad = whole byte
		uint8_t byte = 0;
		startWrite();
		for(int16_t j = 0; j < h; j++, y++) {
			for(int16_t i = 0; i < w; i++) {
				if(i & 7)
					byte <<= 1;
				else
					byte = mask[j * bw + i / 8];
				if(byte & 0x80) {
					writePixel(x + i, y, bitmap[j * w + i]);
				}
			}
		}
		endWrite();
	}

// TEXT- AND CHARACTER-HANDLING FUNCTIONS ----------------------------------

// Draw a character
	void GfxBase::drawChar(int16_t x, int16_t y, unsigned char c,
	                       uint16_t color, uint16_t bg, uint8_t size)
	{

		if(!gfxFont) { // 'Classic' built-in font

			if((x >= _width) || // Clip right
			   (y >= _height) || // Clip bottom
			   ((x + 6 * size - 1) < 0) || // Clip left
			   ((y + 8 * size - 1) < 0))   // Clip top
				return;

			if(!_cp437 && (c >= 176))
				c++; // Handle 'classic' charset behavior

			startWrite();
			for(int8_t i = 0; i < 5; i++) { // Char bitmap = 5 columns
				uint8_t line = pgm_read_byte(&default_font[c * 5 + i]);
				for(int8_t j = 0; j < 8; j++, line >>= 1) {
					if(line & 1) {
						if(size == 1)
							writePixel(x + i, y + j, color);
						else
							writeFillRect(x + i * size, y + j * size, size, size, color);
					} else if(bg != color) {
						if(size == 1)
							writePixel(x + i, y + j, bg);
						else
							writeFillRect(x + i * size, y + j * size, size, size, bg);
					}
				}
			}
			if(bg != color) { // If opaque, draw vertical line for last column
				if(size == 1)
					writeFastVLine(x + 5, y, 8, bg);
				else
					writeFillRect(x + 5 * size, y, size, 8 * size, bg);
			}
			endWrite();

		} else { // Custom font

			// Character is assumed previously filtered by write() to eliminate
			// newlines, returns, non-printable characters, etc.  Calling
			// drawChar() directly with 'bad' characters of font may cause mayhem!

			c -= (uint8_t) pgm_read_byte(&gfxFont->first);
			GFXglyph *glyph = &((pgm_read_pointer(&gfxFont->glyph, GFXglyph))[c]);
			uint8_t *bitmap = pgm_read_pointer(&gfxFont->bitmap, uint8_t);

			uint16_t bo = pgm_read_word(&glyph->bitmapOffset);
			uint8_t w = pgm_read_byte(&glyph->width),
					h = pgm_read_byte(&glyph->height);
			int8_t xo = pgm_read_byte(&glyph->xOffset),
					yo = pgm_read_byte(&glyph->yOffset);
			uint8_t xx, yy, bits = 0, bit = 0;
			int16_t xo16 = 0, yo16 = 0;

			if(size > 1) {
				xo16 = xo;
				yo16 = yo;
			}

			// Todo: Add character clipping here

			// NOTE: THERE IS NO 'BACKGROUND' COLOR OPTION ON CUSTOM FONTS.
			// THIS IS ON PURPOSE AND BY DESIGN.  The background color feature
			// has typically been used with the 'classic' font to overwrite old
			// screen contents with new data.  This ONLY works because the
			// characters are a uniform size; it's not a sensible thing to do with
			// proportionally-spaced fonts with glyphs of varying sizes (and that
			// may overlap).  To replace previously-drawn text when using a custom
			// font, use the getTextBounds() function to determine the smallest
			// rectangle encompassing a string, erase the area with fillRect(),
			// then draw new text.  This WILL infortunately 'blink' the text, but
			// is unavoidable.  Drawing 'background' pixels will NOT fix this,
			// only creates a new set of problems.  Have an idea to work around
			// this (a canvas object type for MCUs that can afford the RAM and
			// displays supporting setAddrWindow() and pushColors()), but haven't
			// implemented this yet.

			startWrite();
			for(yy = 0; yy < h; yy++) {
				for(xx = 0; xx < w; xx++) {
					if(!(bit++ & 7)) {
						bits = pgm_read_byte(&bitmap[bo++]);
					}
					if(bits & 0x80) {
						if(size == 1) {
							writePixel(x + xo + xx, y + yo + yy, color);
						} else {
							writeFillRect(x + (xo16 + xx) * size, y + (yo16 + yy) * size,
							              size, size, color);
						}
					}
					bits <<= 1;
				}
			}
			endWrite();

		} // End classic vs custom font
	}

	size_t GfxBase::write(uint8_t c)
	{
		if(!gfxFont) { // 'Classic' built-in font

			if(c == '\n') {
				cursor_x = 0;
				cursor_y += textsize * 8;
			} else if(c != '\r') {
				if(wrap && ((cursor_x + textsize * 6) > _width)) {
					cursor_x = 0;
					cursor_y += textsize * 8;
				}
				drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
				cursor_x += textsize * 6;
			}

		} else { // Custom font

			if(c == '\n') {
				cursor_x = 0;
				cursor_y += (int16_t) textsize *
				            (uint8_t) pgm_read_byte(&gfxFont->yAdvance);
			} else if(c != '\r') {
				uint8_t first = pgm_read_byte(&gfxFont->first);
				if((c >= first) && (c <= (uint8_t) pgm_read_byte(&gfxFont->last))) {
					GFXglyph *glyph = &(( pgm_read_pointer( &gfxFont->glyph, GFXglyph))[c - first]);
					uint8_t w = pgm_read_byte(&glyph->width),
							h = pgm_read_byte(&glyph->height);
					if((w > 0) && (h > 0)) { // Is there an associated bitmap?
						int16_t xo = (int8_t) pgm_read_byte(&glyph->xOffset); // sic
						if(wrap && ((cursor_x + textsize * (xo + w)) > _width)) {
							cursor_x = 0;
							cursor_y += (int16_t) textsize *
							            (uint8_t) pgm_read_byte(&gfxFont->yAdvance);
						}
						drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
					}
					cursor_x += (uint8_t) pgm_read_byte(&glyph->xAdvance) * (int16_t) textsize;
				}
			}

		}

		return 1;
	}

	void GfxBase::setCursor(int16_t x, int16_t y)
	{
		cursor_x = x;
		cursor_y = y;
	}

	int16_t GfxBase::getCursorX(void) const
	{
		return cursor_x;
	}

	int16_t GfxBase::getCursorY(void) const
	{
		return cursor_y;
	}

	void GfxBase::setTextSize(uint8_t s)
	{
		textsize = (s > 0) ? s : 1;
	}

	void GfxBase::setTextColor(uint16_t c)
	{
		textcolor = textbgcolor = c;
	}

	void GfxBase::setTextColor(uint16_t c, uint16_t b)
	{
		textcolor = c;
		textbgcolor = b;
	}

	void GfxBase::setTextWrap(bool w)
	{
		this->wrap = w;
	}

	uint8_t GfxBase::getRotation(void) const
	{
		return this->rotation;
	}

	void GfxBase::setRotation(uint8_t x)
	{
		this->rotation = x & 3;

		switch(this->rotation) {
		default:
		case 0:
		case 2:
			_width = WIDTH;
			_height = HEIGHT;
			break;

		case 1:
		case 3:
			_width = HEIGHT;
			_height = WIDTH;
			break;
		}
	}

	void GfxBase::cp437(bool x)
	{
		_cp437 = x;
	}

	void GfxBase::setFont(const GFXfont *f)
	{
		if(f) {
			if(!gfxFont)
				cursor_y += 6;
		} else if(gfxFont) {
			cursor_y -= 6;
		}

		gfxFont = (GFXfont *) f;
	}

	void GfxBase::charBounds(char c, int16_t *x, int16_t *y, int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy)
	{
		if(gfxFont) {

			if(c == '\n') {
				*x = 0;
				*y += textsize * (uint8_t) pgm_read_byte(&gfxFont->yAdvance);
			} else if(c != '\r') { // Not a carriage return; is normal char
				uint8_t first = pgm_read_byte(&gfxFont->first),
						last = pgm_read_byte(&gfxFont->last);
				if((c >= first) && (c <= last)) {
					GFXglyph *glyph = &((pgm_read_pointer( &gfxFont->glyph, GFXglyph))[c - first]);
					uint8_t gw = pgm_read_byte(&glyph->width),
							gh = pgm_read_byte(&glyph->height),
							xa = pgm_read_byte(&glyph->xAdvance);
					int8_t xo = pgm_read_byte(&glyph->xOffset),
							yo = pgm_read_byte(&glyph->yOffset);
					if(wrap && ((*x + (((int16_t) xo + gw) * textsize)) > _width)) {
						*x = 0;
						*y += textsize * (uint8_t) pgm_read_byte(&gfxFont->yAdvance);
					}
					int16_t ts = (int16_t) textsize,
							x1 = *x + xo * ts,
							y1 = *y + yo * ts,
							x2 = x1 + gw * ts - 1,
							y2 = y1 + gh * ts - 1;
					if(x1 < *minx)
						*minx = x1;
					if(y1 < *miny)
						*miny = y1;
					if(x2 > *maxx)
						*maxx = x2;
					if(y2 > *maxy)
						*maxy = y2;
					*x += xa * ts;
				}
			}

		} else { // Default font

			if(c == '\n') {                     // Newline?
				*x = 0;                        // Reset x to zero,
				*y += textsize * 8;             // advance y one line
				// min/max x/y unchaged -- that waits for next 'normal' character
			} else if(c != '\r') {  // Normal char; ignore carriage returns
				if(wrap && ((*x + textsize * 6) > _width)) { // Off right?
					*x = 0;                    // Reset x to zero,
					*y += textsize * 8;         // advance y one line
				}
				int x2 = *x + textsize * 6 - 1, // Lower-right pixel of char
						y2 = *y + textsize * 8 - 1;
				if(x2 > *maxx)
					*maxx = x2;      // Track max x, y
				if(y2 > *maxy)
					*maxy = y2;
				if(*x < *minx)
					*minx = *x;      // Track min x, y
				if(*y < *miny)
					*miny = *y;
				*x += textsize * 6;             // Advance x one char
			}
		}
	}

// Pass string and a cursor position, returns UL corner and W,H.
	void GfxBase::getTextBounds(char *str, int16_t x, int16_t y,
	                            int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h)
	{
		uint8_t c; // Current character

		*x1 = x;
		*y1 = y;
		*w = *h = 0;

		int16_t minx = _width, miny = _height, maxx = -1, maxy = -1;

		while((c = *str++))
			charBounds(c, &x, &y, &minx, &miny, &maxx, &maxy);

		if(maxx >= minx) {
			*x1 = minx;
			*w = maxx - minx + 1;
		}
		if(maxy >= miny) {
			*y1 = miny;
			*h = maxy - miny + 1;
		}
	}

	int16_t GfxBase::width(void) const
	{
		return _width;
	}

	int16_t GfxBase::height(void) const
	{
		return _height;
	}

	void GfxBase::invert(bool invert)
	{
	}
}
