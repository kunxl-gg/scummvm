
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

#ifndef BAGEL_BOFLIB_GUI_EDIT_TEXT_H
#define BAGEL_BOFLIB_GUI_EDIT_TEXT_H

#include "bagel/boflib/gui/window.h"
#include "bagel/boflib/string.h"

#if BOF_MAC
#include <TextEdit.h>
#endif

namespace Bagel {

class CBofEditText : public CBofWindow {
public:
#if BOF_WINDOWS
	CBofEditText() {}
#elif BOF_MAC
	CBofEditText() { m_hTE = nullptr; }
#endif

	CBofEditText(const CHAR *pszName, INT x = 0, INT y = 0, INT nWidth = USE_DEFAULT, INT nHeight = USE_DEFAULT, CBofWindow *pParent = nullptr);

	ERROR_CODE Create(const CHAR *pszName, INT x = 0, INT y = 0, INT nWidth = USE_DEFAULT, INT nHeight = USE_DEFAULT, CBofWindow *pParent = nullptr, UINT nControlID = 0);
	ERROR_CODE Create(const CHAR *pszName, CBofRect *pRect = nullptr, CBofWindow *pParent = nullptr, UINT nControlID = 0);

	CBofString GetText();
	VOID SetText(const CHAR *pszString);

#if BOF_MAC
	virtual VOID Destroy();
	virtual VOID OnPaint(CBofRect *pRect);
	virtual VOID OnActivate();
	virtual VOID OnDeActivate();
	virtual VOID OnLButtonDown(UINT nFlags, CBofPoint *pPoint);
	VOID OnSelect();
	VOID Key(const CHAR key);
#endif

protected:
#if BOF_MAC
	TEHandle m_hTE;
#endif
};

} // namespace Bagel

#endif
