/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 *
 */

#include "common/file.h"
#include "common/stdafx.h"

#include "graphics/cursorman.h"

#include "agi/agi.h"
#include "agi/graphics.h"

namespace Agi {

#define DEV_X0(x) ((x) << 1)
#define DEV_X1(x) (((x) << 1) + 1)
#define DEV_Y(x) (x)

#ifndef MAX_INT
#  define MAX_INT (int)((unsigned)~0 >> 1)
#endif

#include "agi/font.h"

/**
 * 16 color RGB palette (plus 16 transparent colors).
 * This array contains the 6-bit RGB values of the EGA palette exported
 * to the console drivers.
 */
uint8 egaPalette[16 * 3] = {
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x2a,
	0x00, 0x2a, 0x00,
	0x00, 0x2a, 0x2a,
	0x2a, 0x00, 0x00,
	0x2a, 0x00, 0x2a,
	0x2a, 0x15, 0x00,
	0x2a, 0x2a, 0x2a,
	0x15, 0x15, 0x15,
	0x15, 0x15, 0x3f,
	0x15, 0x3f, 0x15,
	0x15, 0x3f, 0x3f,
	0x3f, 0x15, 0x15,
	0x3f, 0x15, 0x3f,
	0x3f, 0x3f, 0x15,
	0x3f, 0x3f, 0x3f
};

/**
 * 16 color amiga-ish palette.
 */
uint8 newPalette[16 * 3] = {
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x3f,
	0x00, 0x2A, 0x00,
	0x00, 0x2A, 0x2A,
	0x33, 0x00, 0x00,
	0x2f, 0x1c, 0x37,
	0x23, 0x14, 0x00,
	0x2f, 0x2f, 0x2f,
	0x15, 0x15, 0x15,
	0x00, 0x2f, 0x3f,
	0x00, 0x33, 0x15,
	0x15, 0x3F, 0x3F,
	0x3f, 0x27, 0x23,
	0x3f, 0x15, 0x3f,
	0x3b, 0x3b, 0x00,
	0x3F, 0x3F, 0x3F
};

/**
 * 256 color palette used with AGI256 and AGI256-2 games.
 * Uses full 8 bits per color component.
 */
uint8 vgaPalette[256 * 3] = {
	0x00, 0x00, 0x00, 
	0x00, 0x00, 0xA8, 
	0x00, 0xA8, 0x00, 
	0x00, 0xA8, 0xA8, 
	0xA8, 0x00, 0x00, 
	0xA8, 0x00, 0xA8, 
	0xA8, 0x54, 0x00, 
	0xA8, 0xA8, 0xA8, 
	0x54, 0x54, 0x54, 
	0x54, 0x54, 0xFC, 
	0x54, 0xFC, 0x54, 
	0x54, 0xFC, 0xFC, 
	0xFC, 0x54, 0x54, 
	0xFC, 0x54, 0xFC, 
	0xFC, 0xFC, 0x54, 
	0xFC, 0xFC, 0xFC, 
	0x00, 0x00, 0x00, 
	0x14, 0x14, 0x14, 
	0x20, 0x20, 0x20, 
	0x2C, 0x2C, 0x2C, 
	0x38, 0x38, 0x38, 
	0x44, 0x44, 0x44, 
	0x50, 0x50, 0x50, 
	0x60, 0x60, 0x60, 
	0x70, 0x70, 0x70, 
	0x80, 0x80, 0x80, 
	0x90, 0x90, 0x90, 
	0xA0, 0xA0, 0xA0, 
	0xB4, 0xB4, 0xB4, 
	0xC8, 0xC8, 0xC8, 
	0xE0, 0xE0, 0xE0, 
	0xFC, 0xFC, 0xFC, 
	0x00, 0x00, 0xFC, 
	0x40, 0x00, 0xFC, 
	0x7C, 0x00, 0xFC, 
	0xBC, 0x00, 0xFC, 
	0xFC, 0x00, 0xFC, 
	0xFC, 0x00, 0xBC, 
	0xFC, 0x00, 0x7C, 
	0xFC, 0x00, 0x40, 
	0xFC, 0x00, 0x00, 
	0xFC, 0x40, 0x00, 
	0xFC, 0x7C, 0x00, 
	0xFC, 0xBC, 0x00, 
	0xFC, 0xFC, 0x00, 
	0xBC, 0xFC, 0x00, 
	0x7C, 0xFC, 0x00, 
	0x40, 0xFC, 0x00, 
	0x00, 0xFC, 0x00, 
	0x00, 0xFC, 0x40, 
	0x00, 0xFC, 0x7C, 
	0x00, 0xFC, 0xBC, 
	0x00, 0xFC, 0xFC, 
	0x00, 0xBC, 0xFC, 
	0x00, 0x7C, 0xFC, 
	0x00, 0x40, 0xFC, 
	0x7C, 0x7C, 0xFC, 
	0x9C, 0x7C, 0xFC, 
	0xBC, 0x7C, 0xFC, 
	0xDC, 0x7C, 0xFC, 
	0xFC, 0x7C, 0xFC, 
	0xFC, 0x7C, 0xDC, 
	0xFC, 0x7C, 0xBC, 
	0xFC, 0x7C, 0x9C, 
	0xFC, 0x7C, 0x7C, 
	0xFC, 0x9C, 0x7C, 
	0xFC, 0xBC, 0x7C, 
	0xFC, 0xDC, 0x7C, 
	0xFC, 0xFC, 0x7C, 
	0xDC, 0xFC, 0x7C, 
	0xBC, 0xFC, 0x7C, 
	0x9C, 0xFC, 0x7C, 
	0x7C, 0xFC, 0x7C, 
	0x7C, 0xFC, 0x9C, 
	0x7C, 0xFC, 0xBC, 
	0x7C, 0xFC, 0xDC, 
	0x7C, 0xFC, 0xFC, 
	0x7C, 0xDC, 0xFC, 
	0x7C, 0xBC, 0xFC, 
	0x7C, 0x9C, 0xFC, 
	0xB4, 0xB4, 0xFC, 
	0xC4, 0xB4, 0xFC, 
	0xD8, 0xB4, 0xFC, 
	0xE8, 0xB4, 0xFC, 
	0xFC, 0xB4, 0xFC, 
	0xFC, 0xB4, 0xE8, 
	0xFC, 0xB4, 0xD8, 
	0xFC, 0xB4, 0xC4, 
	0xFC, 0xB4, 0xB4, 
	0xFC, 0xC4, 0xB4, 
	0xFC, 0xD8, 0xB4, 
	0xFC, 0xE8, 0xB4, 
	0xFC, 0xFC, 0xB4, 
	0xE8, 0xFC, 0xB4, 
	0xD8, 0xFC, 0xB4, 
	0xC4, 0xFC, 0xB4, 
	0xB4, 0xFC, 0xB4, 
	0xB4, 0xFC, 0xC4, 
	0xB4, 0xFC, 0xD8, 
	0xB4, 0xFC, 0xE8, 
	0xB4, 0xFC, 0xFC, 
	0xB4, 0xE8, 0xFC, 
	0xB4, 0xD8, 0xFC, 
	0xB4, 0xC4, 0xFC, 
	0x00, 0x00, 0x70, 
	0x1C, 0x00, 0x70, 
	0x38, 0x00, 0x70, 
	0x54, 0x00, 0x70, 
	0x70, 0x00, 0x70, 
	0x70, 0x00, 0x54, 
	0x70, 0x00, 0x38, 
	0x70, 0x00, 0x1C, 
	0x70, 0x00, 0x00, 
	0x70, 0x1C, 0x00, 
	0x70, 0x38, 0x00, 
	0x70, 0x54, 0x00, 
	0x70, 0x70, 0x00, 
	0x54, 0x70, 0x00, 
	0x38, 0x70, 0x00, 
	0x1C, 0x70, 0x00, 
	0x00, 0x70, 0x00, 
	0x00, 0x70, 0x1C, 
	0x00, 0x70, 0x38, 
	0x00, 0x70, 0x54, 
	0x00, 0x70, 0x70, 
	0x00, 0x54, 0x70, 
	0x00, 0x38, 0x70, 
	0x00, 0x1C, 0x70, 
	0x38, 0x38, 0x70, 
	0x44, 0x38, 0x70, 
	0x54, 0x38, 0x70, 
	0x60, 0x38, 0x70, 
	0x70, 0x38, 0x70, 
	0x70, 0x38, 0x60, 
	0x70, 0x38, 0x54, 
	0x70, 0x38, 0x44, 
	0x70, 0x38, 0x38, 
	0x70, 0x44, 0x38, 
	0x70, 0x54, 0x38, 
	0x70, 0x60, 0x38, 
	0x70, 0x70, 0x38, 
	0x60, 0x70, 0x38, 
	0x54, 0x70, 0x38, 
	0x44, 0x70, 0x38, 
	0x38, 0x70, 0x38, 
	0x38, 0x70, 0x44, 
	0x38, 0x70, 0x54, 
	0x38, 0x70, 0x60, 
	0x38, 0x70, 0x70, 
	0x38, 0x60, 0x70, 
	0x38, 0x54, 0x70, 
	0x38, 0x44, 0x70, 
	0x50, 0x50, 0x70, 
	0x58, 0x50, 0x70, 
	0x60, 0x50, 0x70, 
	0x68, 0x50, 0x70, 
	0x70, 0x50, 0x70, 
	0x70, 0x50, 0x68, 
	0x70, 0x50, 0x60, 
	0x70, 0x50, 0x58, 
	0x70, 0x50, 0x50, 
	0x70, 0x58, 0x50, 
	0x70, 0x60, 0x50, 
	0x70, 0x68, 0x50, 
	0x70, 0x70, 0x50, 
	0x68, 0x70, 0x50, 
	0x60, 0x70, 0x50, 
	0x58, 0x70, 0x50, 
	0x50, 0x70, 0x50, 
	0x50, 0x70, 0x58, 
	0x50, 0x70, 0x60, 
	0x50, 0x70, 0x68, 
	0x50, 0x70, 0x70, 
	0x50, 0x68, 0x70, 
	0x50, 0x60, 0x70, 
	0x50, 0x58, 0x70, 
	0x00, 0x00, 0x40, 
	0x10, 0x00, 0x40, 
	0x20, 0x00, 0x40, 
	0x30, 0x00, 0x40, 
	0x40, 0x00, 0x40, 
	0x40, 0x00, 0x30, 
	0x40, 0x00, 0x20, 
	0x40, 0x00, 0x10, 
	0x40, 0x00, 0x00, 
	0x40, 0x10, 0x00, 
	0x40, 0x20, 0x00, 
	0x40, 0x30, 0x00, 
	0x40, 0x40, 0x00, 
	0x30, 0x40, 0x00, 
	0x20, 0x40, 0x00, 
	0x10, 0x40, 0x00, 
	0x00, 0x40, 0x00, 
	0x00, 0x40, 0x10, 
	0x00, 0x40, 0x20, 
	0x00, 0x40, 0x30, 
	0x00, 0x40, 0x40, 
	0x00, 0x30, 0x40, 
	0x00, 0x20, 0x40, 
	0x00, 0x10, 0x40, 
	0x20, 0x20, 0x40, 
	0x28, 0x20, 0x40, 
	0x30, 0x20, 0x40, 
	0x38, 0x20, 0x40, 
	0x40, 0x20, 0x40, 
	0x40, 0x20, 0x38, 
	0x40, 0x20, 0x30, 
	0x40, 0x20, 0x28, 
	0x40, 0x20, 0x20, 
	0x40, 0x28, 0x20, 
	0x40, 0x30, 0x20, 
	0x40, 0x38, 0x20, 
	0x40, 0x40, 0x20, 
	0x38, 0x40, 0x20, 
	0x30, 0x40, 0x20, 
	0x28, 0x40, 0x20, 
	0x20, 0x40, 0x20, 
	0x20, 0x40, 0x28, 
	0x20, 0x40, 0x30, 
	0x20, 0x40, 0x38, 
	0x20, 0x40, 0x40, 
	0x20, 0x38, 0x40, 
	0x20, 0x30, 0x40, 
	0x20, 0x28, 0x40, 
	0x2C, 0x2C, 0x40, 
	0x30, 0x2C, 0x40, 
	0x34, 0x2C, 0x40, 
	0x3C, 0x2C, 0x40, 
	0x40, 0x2C, 0x40, 
	0x40, 0x2C, 0x3C, 
	0x40, 0x2C, 0x34, 
	0x40, 0x2C, 0x30, 
	0x40, 0x2C, 0x2C, 
	0x40, 0x30, 0x2C, 
	0x40, 0x34, 0x2C, 
	0x40, 0x3C, 0x2C, 
	0x40, 0x40, 0x2C, 
	0x3C, 0x40, 0x2C, 
	0x34, 0x40, 0x2C, 
	0x30, 0x40, 0x2C, 
	0x2C, 0x40, 0x2C, 
	0x2C, 0x40, 0x30, 
	0x2C, 0x40, 0x34, 
	0x2C, 0x40, 0x3C, 
	0x2C, 0x40, 0x40, 
	0x2C, 0x3C, 0x40, 
	0x2C, 0x34, 0x40, 
	0x2C, 0x30, 0x40, 
	0x40, 0x40, 0x40, 
	0x38, 0x38, 0x38, 
	0x30, 0x30, 0x30, 
	0x28, 0x28, 0x28, 
	0x24, 0x24, 0x24, 
	0x1C, 0x1C, 0x1C, 
	0x14, 0x14, 0x14, 
	0x0C, 0x0C, 0x0C
};

static uint16 cgaMap[16] = {
	0x0000,			/*  0 - black */
	0x0d00,			/*  1 - blue */
	0x0b00,			/*  2 - green */
	0x0f00,			/*  3 - cyan */
	0x000b,			/*  4 - red */
	0x0b0d,			/*  5 - magenta */
	0x000d,			/*  6 - brown */
	0x0b0b,			/*  7 - gray */
	0x0d0d,			/*  8 - dark gray */
	0x0b0f,			/*  9 - light blue */
	0x0b0d,			/* 10 - light green */
	0x0f0d,			/* 11 - light cyan */
	0x0f0d,			/* 12 - light red */
	0x0f00,			/* 13 - light magenta */
	0x0f0b,			/* 14 - yellow */
	0x0f0f			/* 15 - white */
};

struct UpdateBlock {
	int x1, y1;
	int x2, y2;
};

static struct UpdateBlock update = {
	MAX_INT, MAX_INT, 0, 0
};

GfxMgr::GfxMgr(AgiEngine *vm) : _vm(vm) {
	_shakeH = NULL;
	_shakeV = NULL;
	_agipalFileNum = 0;
}


/*
 *  Layer 4:  640x480?  ==================  User display
 *                              ^
 *                              |  do_update(), put_block()
 *                              |
 *  Layer 3:  640x480?  ==================  Framebuffer
 *                              ^
 *                              |  flush_block(), put_pixels()
 *                              |
 *  Layer 2:  320x200   ==================  AGI engine screen (console), put_pixel()
 *                              |
 *  Layer 1:  160x336   ==================  AGI screen
 *
 *  Upper half (160x168) of Layer 1 is for the normal 16 color & control line/priority info.
 *  Lower half (160x168) of Layer 1 is for the 256 color information (Used with AGI256 & AGI256-2).
 */

#define SHAKE_MAG 3

void GfxMgr::shakeStart() {
	int i;

	if ((_shakeH = (uint8 *)malloc(GFX_WIDTH * SHAKE_MAG)) == NULL)
		return;

	if ((_shakeV = (uint8 *)malloc(SHAKE_MAG * (GFX_HEIGHT - SHAKE_MAG))) == NULL) {
		free(_shakeH);
		return;
	}

	for (i = 0; i < GFX_HEIGHT - SHAKE_MAG; i++) {
		memcpy(_shakeV + i * SHAKE_MAG, _agiScreen + i * GFX_WIDTH, SHAKE_MAG);
	}

	for (i = 0; i < SHAKE_MAG; i++) {
		memcpy(_shakeH + i * GFX_WIDTH, _agiScreen + i * GFX_WIDTH, GFX_WIDTH);
	}
}

void GfxMgr::shakeScreen(int n) {
	int i;

	if (n == 0) {
		for (i = 0; i < (GFX_HEIGHT - SHAKE_MAG); i++) {
			memmove(&_agiScreen[GFX_WIDTH * i],
					&_agiScreen[GFX_WIDTH * (i + SHAKE_MAG) + SHAKE_MAG],
					GFX_WIDTH - SHAKE_MAG);
		}
	} else {
		for (i = GFX_HEIGHT - SHAKE_MAG - 1; i >= 0; i--) {
			memmove(&_agiScreen[GFX_WIDTH * (i + SHAKE_MAG) + SHAKE_MAG],
					&_agiScreen[GFX_WIDTH * i], GFX_WIDTH - SHAKE_MAG);
		}
	}
}

void GfxMgr::shakeEnd() {
	int i;

	for (i = 0; i < GFX_HEIGHT - SHAKE_MAG; i++) {
		memcpy(_agiScreen + i * GFX_WIDTH, _shakeV + i * SHAKE_MAG, SHAKE_MAG);
	}

	for (i = 0; i < SHAKE_MAG; i++) {
		memcpy(_agiScreen + i * GFX_WIDTH, _shakeH + i * GFX_WIDTH, GFX_WIDTH);
	}

	flushBlock(0, 0, GFX_WIDTH - 1, GFX_HEIGHT - 1);

	free(_shakeV);
	free(_shakeH);
}

void GfxMgr::putTextCharacter(int l, int x, int y, unsigned int c, int fg, int bg, bool checkerboard) {
	int x1, y1, xx, yy, cc;
	const uint8 *p;

	p = Agi::curFont + ((unsigned int)c * CHAR_LINES);
	for (y1 = 0; y1 < CHAR_LINES; y1++) {
		for (x1 = 0; x1 < CHAR_COLS; x1++) {
			xx = x + x1;
			yy = y + y1;
			cc = (*p & (1 << (7 - x1))) ? fg : bg;
			_agiScreen[xx + yy * GFX_WIDTH] = cc;
		}

		p++;
	}

	// Simple checkerboard effect to simulate "greyed out" text.
	// This is what Sierra's interpreter does for things like menu items
	// that aren't selectable (such as separators). -- dsymonds
	if (checkerboard) {
		for (yy = y; yy < y + CHAR_LINES; yy++)
			for (xx = x + (~yy & 1); xx < x + CHAR_COLS; xx += 2)
				_agiScreen[xx + yy * GFX_WIDTH] = 15;
	}

	/* FIXME: we don't want this when we're writing on the
	 *        console!
	 */
	flushBlock(x, y, x + CHAR_COLS - 1, y + CHAR_LINES - 1);
}

void GfxMgr::drawRectangle(int x1, int y1, int x2, int y2, int c) {
	int y, w, h;
	uint8 *p0;

	if (x1 >= GFX_WIDTH)
		x1 = GFX_WIDTH - 1;
	if (y1 >= GFX_HEIGHT)
		y1 = GFX_HEIGHT - 1;
	if (x2 >= GFX_WIDTH)
		x2 = GFX_WIDTH - 1;
	if (y2 >= GFX_HEIGHT)
		y2 = GFX_HEIGHT - 1;

	w = x2 - x1 + 1;
	h = y2 - y1 + 1;
	p0 = &_agiScreen[x1 + y1 * GFX_WIDTH];
	for (y = 0; y < h; y++) {
		memset(p0, c, w);
		p0 += GFX_WIDTH;
	}
}

void GfxMgr::drawFrame(int x1, int y1, int x2, int y2, int c1, int c2) {
	int y, w;
	uint8 *p0;

	/* top line */
	w = x2 - x1 + 1;
	p0 = &_agiScreen[x1 + y1 * GFX_WIDTH];
	memset(p0, c1, w);

	/* bottom line */
	p0 = &_agiScreen[x1 + y2 * GFX_WIDTH];
	memset(p0, c2, w);

	/* side lines */
	for (y = y1; y <= y2; y++) {
		_agiScreen[x1 + y * GFX_WIDTH] = c1;
		_agiScreen[x2 + y * GFX_WIDTH] = c2;
	}
}

void GfxMgr::drawBox(int x1, int y1, int x2, int y2, int colour1, int colour2, int m) {
	x1 += m;
	y1 += m;
	x2 -= m;
	y2 -= m;

	drawRectangle(x1, y1, x2, y2, colour1);
	drawFrame(x1 + 2, y1 + 2, x2 - 2, y2 - 2, colour2, colour2);
	flushBlock(x1, y1, x2, y2);
}

void GfxMgr::printCharacter(int x, int y, char c, int fg, int bg) {
	x *= CHAR_COLS;
	y *= CHAR_LINES;

	putTextCharacter(0, x, y, c, fg, bg);
	// redundant! already inside put_text_character!
	// flush_block (x, y, x + CHAR_COLS - 1, y + CHAR_LINES - 1);
}

/**
 * Draw a default style button.
 * Swaps background and foreground color if button is in focus or being pressed.
 * @param x  x coordinate of the button
 * @param y  y coordinate of the button
 * @param a  set if the button has focus
 * @param p  set if the button is pressed
 * @param fgcolor foreground color of the button when it is neither in focus nor being pressed
 * @param bgcolor background color of the button when it is neither in focus nor being pressed
 */
void GfxMgr::drawDefaultStyleButton(int x, int y, const char *s, int a, int p, int fgcolor, int bgcolor) {
	int textOffset     = _vm->_defaultButtonStyle.getTextOffset(a > 0, p > 0);
	AgiTextColor color = _vm->_defaultButtonStyle.getColor     (a > 0, p > 0, fgcolor, bgcolor);
	bool border        = _vm->_defaultButtonStyle.getBorder    (a > 0, p > 0);

	rawDrawButton(x, y, s, color.fg, color.bg, border, textOffset);
}

/**
 * Draw a button using the currently chosen style.
 * Amiga-style is used for the Amiga-rendering mode, PC-style is used otherwise.
 * @param x  x coordinate of the button
 * @param y  y coordinate of the button
 * @param hasFocus  set if the button has focus
 * @param pressed  set if the button is pressed
 * @param positive  set if button is positive, otherwise button is negative (Only matters with Amiga-style buttons)
 * TODO: Make Amiga-style buttons a bit wider as they were in Amiga AGI games.
 */
void GfxMgr::drawCurrentStyleButton(int x, int y, const char *label, bool hasFocus, bool pressed, bool positive) {
	int textOffset     = _vm->_buttonStyle.getTextOffset(hasFocus, pressed);
	AgiTextColor color = _vm->_buttonStyle.getColor(hasFocus, pressed, positive);
	bool border        = _vm->_buttonStyle.getBorder(hasFocus, pressed);
	
	rawDrawButton(x, y, label, color.fg, color.bg, border, textOffset);
}

void GfxMgr::rawDrawButton(int x, int y, const char *s, int fgcolor, int bgcolor, bool border, int textOffset) {
	int len = strlen(s);
	int x1, y1, x2, y2;

	x1 = x - 3;
	y1 = y - 3;
	x2 = x + CHAR_COLS * len + 2;
	y2 = y + CHAR_LINES + 2;

	// Draw a filled rectangle that's larger than the button. Used for drawing
	// a border around the button as the button itself is drawn after this.
	drawRectangle(x1, y1, x2, y2, border ? BUTTON_BORDER : MSG_BOX_COLOUR);
	
	while (*s) {
		putTextCharacter(0, x + textOffset, y + textOffset, *s++, fgcolor, bgcolor);
		x += CHAR_COLS;
	}

	x1 -= 2;
	y1 -= 2;
	x2 += 2;
	y2 += 2;

	flushBlock(x1, y1, x2, y2);
}

int GfxMgr::testButton(int x, int y, const char *s) {
	int len = strlen(s);
	int x1, y1, x2, y2;

	x1 = x - 3;
	y1 = y - 3;
	x2 = x + CHAR_COLS * len + 2;
	y2 = y + CHAR_LINES + 2;

	if ((int)g_mouse.x >= x1 && (int)g_mouse.y >= y1 && (int)g_mouse.x <= x2 && (int)g_mouse.y <= y2)
		return true;

	return false;
}

void GfxMgr::putBlock(int x1, int y1, int x2, int y2) {
	gfxPutBlock(x1, y1, x2, y2);
}

void GfxMgr::putScreen() {
	putBlock(0, 0, GFX_WIDTH - 1, GFX_HEIGHT - 1);
}

void GfxMgr::pollTimer() {
	_vm->agiTimerLow();
}

int GfxMgr::getKey() {
	return _vm->agiGetKeypressLow();
}

int GfxMgr::keypress() {
	return _vm->agiIsKeypressLow();
}

/*
 * Public functions
 */

/**
 * Initialize the color palette
 * This function initializes the color palette using the specified 16-color
 * RGB palette.
 * @param p  A pointer to the 16-color RGB palette.
 */
void GfxMgr::initPalette(uint8 *p) {
	int i;

	for (i = 0; i < 48; i++) {
		_palette[i] = p[i];
	}
}

void GfxMgr::gfxSetPalette() {
	int i;
	byte pal[256 * 4];

	if (!(_vm->getFeatures() & (GF_AGI256 | GF_AGI256_2))) {
		for (i = 0; i < 16; i++) {
			pal[i * 4 + 0] = _palette[i * 3 + 0] << 2;
			pal[i * 4 + 1] = _palette[i * 3 + 1] << 2;
			pal[i * 4 + 2] = _palette[i * 3 + 2] << 2;
			pal[i * 4 + 3] = 0;
		}
		g_system->setPalette(pal, 0, 16);
	} else {
		for (i = 0; i < 256; i++) {
			pal[i * 4 + 0] = vgaPalette[i * 3 + 0];
			pal[i * 4 + 1] = vgaPalette[i * 3 + 1];
			pal[i * 4 + 2] = vgaPalette[i * 3 + 2];
			pal[i * 4 + 3] = 0;
		}
		g_system->setPalette(pal, 0, 256);
	}
}

//Gets AGIPAL Data
void GfxMgr::setAGIPal(int p0) {
	//If 0 from savefile, do not use
	if (p0 == 0)
		return;

	char filename[15];
	sprintf(filename, "pal.%d", p0);

	Common::File agipal;
	if (!agipal.open(filename))
		error("Couldn't open AGIPAL palette file '%s'", filename);

	//Chunk0 holds colors 0-7
	agipal.read(&_agipalPalette[0], 24);

	//Chunk1 is the same as the chunk0

	//Chunk2 chunk holds colors 8-15
	agipal.seek(24, SEEK_CUR);
	agipal.read(&_agipalPalette[24], 24);

	//Chunk3 is the same as the chunk2

	//Chunks4-7 are duplicates of chunks0-3

	if (agipal.ioFailed())
		error("Couldn't read AGIPAL palette from '%s'", filename);

	_agipalFileNum = p0;

	initPalette(_agipalPalette);
	gfxSetPalette();

	debug(1, "Using AGIPAL palette from '%s'", filename);
}

int GfxMgr::getAGIPalFileNum() {
	return _agipalFileNum;
}

/* put a block onto the screen */
void GfxMgr::gfxPutBlock(int x1, int y1, int x2, int y2) {
	if (x1 >= GFX_WIDTH)
		x1 = GFX_WIDTH - 1;
	if (y1 >= GFX_HEIGHT)
		y1 = GFX_HEIGHT - 1;
	if (x2 >= GFX_WIDTH)
		x2 = GFX_WIDTH - 1;
	if (y2 >= GFX_HEIGHT)
		y2 = GFX_HEIGHT - 1;

	g_system->copyRectToScreen(_screen + y1 * 320 + x1, 320, x1, y1, x2 - x1 + 1, y2 - y1 + 1);
}

/**
 * A black and white SCI-style arrow cursor (11x16).
 * 0 = Transparent.
 * 1 = Black (#000000 in 24-bit RGB).
 * 2 = White (#FFFFFF in 24-bit RGB).
 */
static const byte sciMouseCursor[] = {
	1,1,0,0,0,0,0,0,0,0,0,
	1,2,1,0,0,0,0,0,0,0,0,
	1,2,2,1,0,0,0,0,0,0,0,
	1,2,2,2,1,0,0,0,0,0,0,
	1,2,2,2,2,1,0,0,0,0,0,
	1,2,2,2,2,2,1,0,0,0,0,
	1,2,2,2,2,2,2,1,0,0,0,
	1,2,2,2,2,2,2,2,1,0,0,
	1,2,2,2,2,2,2,2,2,1,0,
	1,2,2,2,2,2,2,2,2,2,1,
	1,2,2,2,2,2,1,0,0,0,0,
	1,2,1,0,1,2,2,1,0,0,0,
	1,1,0,0,1,2,2,1,0,0,0,
	0,0,0,0,0,1,2,2,1,0,0,
	0,0,0,0,0,1,2,2,1,0,0,
	0,0,0,0,0,0,1,2,2,1,0
};

/**
 * RGBA-palette for the black and white SCI-style arrow cursor.
 */
static const byte sciMouseCursorPalette[] = {
	0x00, 0x00, 0x00,	0x00, // Black
	0xFF, 0xFF, 0xFF,	0x00  // White
};

/**
 * An Amiga-style arrow cursor (8x11).
 * 0 = Transparent.
 * 1 = Black     (#000000 in 24-bit RGB).
 * 2 = Red       (#DE2021 in 24-bit RGB).
 * 3 = Light red (#FFCFAD in 24-bit RGB).
 */
static const byte amigaMouseCursor[] = {
	2,3,1,0,0,0,0,0,
	2,2,3,1,0,0,0,0,
	2,2,2,3,1,0,0,0,
	2,2,2,2,3,1,0,0,
	2,2,2,2,2,3,1,0,
	2,2,2,2,2,2,3,1,
	2,0,2,2,3,1,0,0,
	0,0,0,2,3,1,0,0,
	0,0,0,2,2,3,1,0,
	0,0,0,0,2,3,1,0,
	0,0,0,0,2,2,3,1
};

/**
 * RGBA-palette for the Amiga-style arrow cursor
 * and the Amiga-style busy cursor.
 */
static const byte amigaMouseCursorPalette[] = {
	0x00, 0x00, 0x00,	0x00, // Black
	0xDE, 0x20, 0x21,	0x00, // Red
	0xFF, 0xCF, 0xAD,	0x00  // Light red
};

/**
 * An Amiga-style busy cursor showing an hourglass (13x16).
 * 0 = Transparent.
 * 1 = Black     (#000000 in 24-bit RGB).
 * 2 = Red       (#DE2021 in 24-bit RGB).
 * 3 = Light red (#FFCFAD in 24-bit RGB).
 */
static const byte busyAmigaMouseCursor[] = {
	1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,2,2,2,2,2,2,2,2,2,2,2,1,
	1,2,2,2,2,2,2,2,2,2,2,2,1,
	0,1,3,3,3,3,3,3,3,3,3,1,0,
	0,0,1,3,3,3,3,3,3,3,1,0,0,
	0,0,0,1,3,3,3,3,3,1,0,0,0,
	0,0,0,0,1,3,3,3,1,0,0,0,0,
	0,0,0,0,0,1,3,1,0,0,0,0,0,
	0,0,0,0,0,1,3,1,0,0,0,0,0,
	0,0,0,0,1,2,3,2,1,0,0,0,0,
	0,0,0,1,2,2,3,2,2,1,0,0,0,
	0,0,1,2,2,2,3,2,2,2,1,0,0,
	0,1,2,2,2,3,3,3,2,2,2,1,0,
	1,3,3,3,3,3,3,3,3,3,3,3,1,
	1,3,3,3,3,3,3,3,3,3,3,3,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1
};

void GfxMgr::setCursor(bool amigaStyleCursor) {
	if (!amigaStyleCursor) {
		CursorMan.replaceCursorPalette(sciMouseCursorPalette, 1, ARRAYSIZE(sciMouseCursorPalette) / 4);
		CursorMan.replaceCursor(sciMouseCursor, 11, 16, 1, 1, 0);
	} else { // amigaStyleCursor
		CursorMan.replaceCursorPalette(amigaMouseCursorPalette, 1, ARRAYSIZE(amigaMouseCursorPalette) / 4);
		CursorMan.replaceCursor(amigaMouseCursor, 8, 11, 1, 1, 0);
	}
}

/**
 * Initialize graphics device.
 *
 * @see deinit_video()
 */
int GfxMgr::initVideo() {
	if (_vm->_renderMode == Common::kRenderEGA)
		initPalette(egaPalette);
	else
		initPalette(newPalette);

	if ((_agiScreen = (uint8 *)calloc(GFX_WIDTH, GFX_HEIGHT)) == NULL)
		return errNotEnoughMemory;

	gfxSetPalette();

	setCursor(_vm->_renderMode == Common::kRenderAmiga);

	return errOK;
}

/**
 * Deinitialize graphics device.
 *
 * @see init_video()
 */
int GfxMgr::deinitVideo() {
	free(_agiScreen);

	return errOK;
}

int GfxMgr::initMachine() {
	_screen = (unsigned char *)malloc(320 * 200);
	_vm->_clockCount = 0;

	return errOK;
}

int GfxMgr::deinitMachine() {
	free(_screen);

	return errOK;
}

/**
 * Write pixels on the output device.
 * This function writes a row of pixels on the output device. Only the
 * lower 4 bits of each pixel in the row will be used, making this
 * function suitable for use with rows from the AGI screen.
 * @param x x coordinate of the row start (AGI coord.)
 * @param y y coordinate of the row start (AGI coord.)
 * @param n number of pixels in the row
 * @param p pointer to the row start in the AGI screen (Always use sbuf16c as base, not sbuf256c)
 * FIXME: CGA rendering doesn't work correctly with AGI256 or AGI256-2.
 */
void GfxMgr::putPixelsA(int x, int y, int n, uint8 *p) {
	const uint rShift = _vm->_debug.priority ? 4 : 0; // Priority information is in the top 4 bits of a byte taken from sbuf16c.

	// Choose the correct screen to read from. If AGI256 or AGI256-2 is used and we're not trying to show the priority information,
	// then choose the 256 color screen, otherwise choose the 16 color screen (Which also has the priority information).
	p += _vm->getFeatures() & (GF_AGI256 | GF_AGI256_2) && !_vm->_debug.priority ? FROM_SBUF16_TO_SBUF256_OFFSET : 0;

	if (_vm->_renderMode == Common::kRenderCGA) {
		for (x *= 2; n--; p++, x += 2) {
			register uint16 q = (cgaMap[(*p & 0xf0) >> 4] << 4) | cgaMap[*p & 0x0f];
			*(uint16 *)&_agiScreen[x + y * GFX_WIDTH] = (q >> rShift) & 0x0f0f;
		}
	} else {
		const uint16 mask = _vm->getFeatures() & (GF_AGI256 | GF_AGI256_2) && !_vm->_debug.priority ? 0xffff : 0x0f0f;
		for (x *= 2; n--; p++, x += 2) {
			register uint16 q = ((uint16) * p << 8) | *p;
			*(uint16 *)&_agiScreen[x + y * GFX_WIDTH] = (q >> rShift) & mask;
		}
	}
}

/**
 * Schedule blocks for blitting on the output device.
 * This function gets the coordinates of a block in the AGI screen and
 * schedule it to be updated in the output device.
 * @param x1 x coordinate of the upper left corner of the block (AGI coord.)
 * @param y1 y coordinate of the upper left corner of the block (AGI coord.)
 * @param x2 x coordinate of the lower right corner of the block (AGI coord.)
 * @param y2 y coordinate of the lower right corner of the block (AGI coord.)
 *
 * @see do_update()
 */
void GfxMgr::scheduleUpdate(int x1, int y1, int x2, int y2) {
	if (x1 < update.x1)
		update.x1 = x1;
	if (y1 < update.y1)
		update.y1 = y1;
	if (x2 > update.x2)
		update.x2 = x2;
	if (y2 > update.y2)
		update.y2 = y2;
}

/**
 * Update scheduled blocks on the output device.
 * This function exposes the blocks scheduled for updating to the output
 * device. Blocks can be scheduled at any point of the AGI cycle.
 *
 * @see schedule_update()
 */
void GfxMgr::doUpdate() {
	if (update.x1 <= update.x2 && update.y1 <= update.y2) {
		gfxPutBlock(update.x1, update.y1, update.x2, update.y2);
	}

	/* reset update block variables */
	update.x1 = MAX_INT;
	update.y1 = MAX_INT;
	update.x2 = 0;
	update.y2 = 0;
}

/**
 * Updates a block of the framebuffer with contents of the AGI engine screen.
 * This function updates a block in the output device with the contents of
 * the AGI engine screen, handling console transparency.
 * @param x1 x coordinate of the upper left corner of the block
 * @param y1 y coordinate of the upper left corner of the block
 * @param x2 x coordinate of the lower right corner of the block
 * @param y2 y coordinate of the lower right corner of the block
 *
 * @see flush_block_a()
 */
void GfxMgr::flushBlock(int x1, int y1, int x2, int y2) {
	int y, w;
	uint8 *p0;

	scheduleUpdate(x1, y1, x2, y2);

	p0 = &_agiScreen[x1 + y1 * GFX_WIDTH];
	w = x2 - x1 + 1;

	for (y = y1; y <= y2; y++) {
		memcpy(_screen + 320 * y + x1, p0, w);
		p0 += GFX_WIDTH;
	}
}

/**
 * Updates a block of the framebuffer receiving AGI picture coordinates.
 * @param x1 x AGI picture coordinate of the upper left corner of the block
 * @param y1 y AGI picture coordinate of the upper left corner of the block
 * @param x2 x AGI picture coordinate of the lower right corner of the block
 * @param y2 y AGI picture coordinate of the lower right corner of the block
 *
 * @see flush_block()
 */
void GfxMgr::flushBlockA(int x1, int y1, int x2, int y2) {
	//y1 += 8;
	//y2 += 8;
	flushBlock(DEV_X0(x1), DEV_Y(y1), DEV_X1(x2), DEV_Y(y2));
}

/**
 * Updates the framebuffer with contents of the AGI engine screen (console-aware).
 * This function updates the output device with the contents of the AGI
 * screen, handling console transparency.
 */
void GfxMgr::flushScreen() {
	flushBlock(0, 0, GFX_WIDTH - 1, GFX_HEIGHT - 1);
}

/**
 * Clear the output device screen (console-aware).
 * This function clears the output device screen and updates the
 * output device. Contents of the AGI screen are left untouched. This
 * function can be used to simulate a switch to a text mode screen in
 * a graphic-only device.
 * @param c  color to clear the screen
 */
void GfxMgr::clearScreen(int c) {
	memset(_agiScreen, c, GFX_WIDTH * GFX_HEIGHT);
	flushScreen();
}

/**
 * Save a block of the AGI engine screen
 */
void GfxMgr::saveBlock(int x1, int y1, int x2, int y2, uint8 *b) {
	uint8 *p0;
	int w, h;

	p0 = &_agiScreen[x1 + GFX_WIDTH * y1];
	w = x2 - x1 + 1;
	h = y2 - y1 + 1;
	while (h--) {
		memcpy(b, p0, w);
		b += w;
		p0 += GFX_WIDTH;
	}
}

/**
 * Restore a block of the AGI engine screen
 */
void GfxMgr::restoreBlock(int x1, int y1, int x2, int y2, uint8 *b) {
	uint8 *p0;
	int w, h;

	p0 = &_agiScreen[x1 + GFX_WIDTH * y1];
	w = x2 - x1 + 1;
	h = y2 - y1 + 1;
	while (h--) {
		memcpy(p0, b, w);
		b += w;
		p0 += GFX_WIDTH;
	}
	flushBlock(x1, y1, x2, y2);
}

} // End of namespace Agi
