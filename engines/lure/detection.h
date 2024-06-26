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

#ifndef LURE_DETECTION_H
#define LURE_DETECTION_H

#include "engines/advancedDetector.h"

namespace Lure {

enum {
	GF_FLOPPY	= 1 << 0,
	GF_EGA		= 1 << 1,
	GF_KONAMI	= 1 << 2,
	GF_LNGUNK	= 1 << 3
};

struct LureGameDescription {
	AD_GAME_DESCRIPTION_HELPERS(desc);

	ADGameDescription desc;

	uint32 features;
};

#define GAMEOPTION_TTS_NARRATOR 	GUIO_GAMEOPTIONS1
#define GAMEOPTION_COPY_PROTECTION	GUIO_GAMEOPTIONS2

} // End of namespace Lure

#endif // LURE_DETECTION_H
