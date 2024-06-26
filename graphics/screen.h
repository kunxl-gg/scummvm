/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef GRAPHICS_SCREEN_H
#define GRAPHICS_SCREEN_H

#include "graphics/managed_surface.h"
#include "graphics/palette.h"
#include "graphics/pixelformat.h"
#include "common/list.h"
#include "common/rect.h"

namespace Graphics {

/**
 * @defgroup graphics_screen Screen
 * @ingroup graphics
 *
 * @brief Screen class for managing the screen.
 *
 * @{
 */

/**
 * Implements a specialised surface that represents the screen.
 * It keeps track of any areas of itself that are updated by drawing
 * calls, and provides an update that method that blits the affected
 * areas to the physical screen
 */
class Screen : public ManagedSurface {
protected:
	/**
	 * List of affected areas of the screen
	 */
	Common::List<Common::Rect> _dirtyRects;
protected:
	/**
	 * Merges together overlapping dirty areas of the screen
	 */
	void mergeDirtyRects();

	/**
	 * Returns the union of two dirty area rectangles
	 */
	bool unionRectangle(Common::Rect &destRect, const Common::Rect &src1, const Common::Rect &src2);
public:
	Screen();
	Screen(int width, int height);
	Screen(int width, int height, PixelFormat pixelFormat);

	/**
	 * Returns true if there are any pending screen updates (dirty areas)
	 */
	bool isDirty() const { return !_dirtyRects.empty(); }

	/**
	 * Marks the whole screen as dirty. This forces the next call to update
	 * to copy the entire screen contents
	 */
	void makeAllDirty();

	/**
	 * Clear the current dirty rects list
	 */
	virtual void clearDirtyRects() { _dirtyRects.clear(); }

	/**
	 * Adds a rectangle to the list of modified areas of the screen during the
	 * current frame
	 */
	virtual void addDirtyRect(const Common::Rect &r);

	/**
	 * Updates the screen by copying any affected areas to the system
	 */
	virtual void update();

	/**
	 * Updates the screen at the end of an update call
	 */
	virtual void updateScreen();

	/**
	 * Return the currently active palette
	 */
	void getPalette(byte palette[PALETTE_SIZE]);

	/**
	 * Return a portion of the currently active palette
	 */
	void getPalette(byte *palette, uint start, uint num);

	/**
	 * Return a portion of the currently active palette as a palette object
	 */
	Graphics::Palette getPalette(uint start = 0, uint num = PALETTE_COUNT) {
		byte tmp[PALETTE_SIZE];
		getPalette(tmp, start, num);
		return Graphics::Palette(tmp, num);
	}

	/**
	 * Set the palette
	 */
	void setPalette(const byte palette[PALETTE_SIZE]);

	/**
	 * Set a subsection of the palette
	 */
	void setPalette(const byte *palette, uint start, uint num);

	/**
	 * Set a palette based on a passed palette object
	 */
	void setPalette(const Graphics::Palette &pal, uint start = 0) {
		setPalette(pal.data(), start, pal.size());
	}

	/**
	 * Clears the current palette, setting all entries to black
	 */
	void clearPalette();
};
 /** @} */
} // End of namespace Graphics

#endif
