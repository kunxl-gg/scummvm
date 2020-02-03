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
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef ULTIMA8_GUMPS_PAGEDGUMP_H
#define ULTIMA8_GUMPS_PAGEDGUMP_H

#include "ultima/ultima8/gumps/modal_gump.h"
#include "ultima/ultima8/misc/p_dynamic_cast.h"

namespace Ultima {
namespace Ultima8 {

class PagedGump : public ModalGump {
public:
	ENABLE_RUNTIME_CLASSTYPE()

	PagedGump(int left, int right, int top, int shape);
	virtual ~PagedGump(void);

	// Init the gump, call after construction
	virtual void InitGump(Gump *newparent, bool take_focus = true) override;
	virtual void Close(bool no_del = false) override;

	// Paint the Gump
	virtual void PaintThis(RenderSurface *, int32 lerp_factor, bool scaled) override;

	virtual bool OnKeyDown(int key, int mod) override;
	virtual void ChildNotify(Gump *child, uint32 message) override;

	//! add a page. Note: g already has to be a child gump.
	void addPage(Gump *g);

	void enableButtons(bool enabled) {
		buttonsEnabled = enabled;
	}

	bool loadData(IDataSource *ids);
protected:
	virtual void saveData(ODataSource *ods) override;
	int leftOff, rightOff, topOff, gumpShape;
	Std::vector<Gump *> gumps;
	Gump *nextButton;
	Gump *prevButton;
	Std::vector<Gump *>::iterator current;
	bool buttonsEnabled;
};

} // End of namespace Ultima8
} // End of namespace Ultima

#endif
