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
 *
 * Based on the original sources
 *   Faery Tale II -- The Halls of the Dead
 *   (c) 1993-1996 The Wyrmkeep Entertainment Co.
 */

#ifndef SAGA2_MESSAGER_H
#define SAGA2_MESSAGER_H

#include "saga2/vdraw.h"
#include "saga2/grequest.h"

namespace Saga2 {

// max filename length
#define MAX_LOG_NAME_LENGTH 260

/****** messager.cpp/Messager [class] *******************************
*
*   NAME
*       Messager [class] -- classes for handling text messages
*
*   FUNCTION
*       The Messager classes are intended to handle text messages
*       generated by the program. Debugging text, status lines,
*       warning and error messages, and log files are some of the
*       implementations of these classes.
*
*
*
*
*   SEE ALSO
*       gError [class]
*
*******************************************************************/

//-------------------------------------------------------------------
// This is the base messager class
//   Note the constructor which takes a string argument. Most of the
//     Messager classes can be enabled based on the value of an
//     entry in the INI file.
//   Carriage returns are automatically appended to lines that don't
//     already have them.

class Messager {
protected:
	bool enabled;
	virtual int dumpit(char *, size_t) = 0;

public:
	Messager() {
		enabled = TRUE;
	}
	Messager(char *entry) {
		enabled = TRUE;
	}
	virtual ~Messager() {}

	size_t operator()(char *format, ...);
	size_t va(const char *format, va_list argptr);

	void enable() {
		enabled = TRUE;
	}
	void disable() {
		enabled = FALSE;
	}
	bool active() {
		return enabled;
	}
};

typedef Messager *pMessager;

//-------------------------------------------------------------------
// This messager writes text on a gDisplayPort (usually mainPort).
//   If you have seen the debugging lines in FTA2, this class acts
//   like a single one of those lines. Generally you will have several
//   of these. By default the lines will be in the lower right corner,
//   but you can specify any location you like. The positions of
//   multiple lines are automatically accounted for.

class StatusLineMessager : public Messager {
private:
	int line;
	int32 atX, atY, atW;
	int16 atColor;
	gDisplayPort *textPort;

protected:
	int dumpit(char *s, size_t size) ;

public:
	StatusLineMessager(int line, gDisplayPort *mp, int32 x = -1, int32 y = -1, int32 w = -1, int16 color = -1);
	StatusLineMessager(char *entry, int line, gDisplayPort *mp, int32 x = -1, int32 y = -1, int32 w = -1, int16 color = -1);
	~StatusLineMessager();
};


} // end of namespace Saga2

#endif
