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

#include "common/str.h"
#include "glk/scott/scott.h"
#include "glk/scott/globals.h"
#include "glk/scott/command_parser.h"
#include "glk/scott/decompress_text.h"
#include "glk/scott/decompress_z80.h"
#include "glk/scott/detection.h"
#include "glk/scott/detection_tables.h"
#include "glk/scott/game_info.h"
#include "glk/scott/hulk.h"
#include "glk/scott/line_drawing.h"
#include "glk/scott/saga_draw.h"
#include "glk/scott/c64_checksums.h"
#include "glk/scott/game_specific.h"
#include "glk/scott/resource.h"
#include "glk/scott/load_game.h"

namespace Glk {
namespace Scott {

void loadZXSpectrum(Common::SeekableReadStream *f, Common::String md5) {
	_G(_entireFile) = new uint8_t[_G(_fileLength)];
	size_t result = f->read(_G(_entireFile), _G(_fileLength));
	if (result != _G(_fileLength))
		g_scott->fatal("File empty or read error!");

	uint8_t *uncompressed = decompressZ80(_G(_entireFile), _G(_fileLength));
	if (uncompressed != nullptr) {
		delete[] _G(_entireFile);
		_G(_entireFile) = uncompressed;
		_G(_fileLength) = 0xc000;
	}

	int offset;
	DictionaryType dict_type = getId(&offset);
	if (dict_type == NOT_A_GAME)
		return;

	int index = _G(_md5Index)[md5];
	if (tryLoading(g_games[index], offset, 0)) {
		delete _G(_game);
		_G(_game) = &g_games[index];
	}
}

void loadC64(Common::SeekableReadStream* f, Common::String md5) {
	_G(_entireFile) = new uint8_t[_G(_fileLength)];
	size_t result = f->read(_G(_entireFile), _G(_fileLength));
	if (result != _G(_fileLength))
		g_scott->fatal("File empty or read error!");

	CURRENT_GAME = static_cast<GameIDType>(detectC64(&_G(_entireFile), &_G(_fileLength)));
}

void loadGameFile(Common::SeekableReadStream *f) {

	for (int i = 0; i < NUMBER_OF_DIRECTIONS; i++)
		_G(_directions)[i] = _G(_englishDirections)[i];
	for (int i = 0; i < NUMBER_OF_SKIPPABLE_WORDS; i++)
		_G(_skipList)[i] = _G(_englishSkipList)[i];
	for (int i = 0; i < NUMBER_OF_DELIMITERS; i++)
		_G(_delimiterList)[i] = _G(_englishDelimiterList)[i];
	for (int i = 0; i < NUMBER_OF_EXTRA_NOUNS; i++)
		_G(_extraNouns)[i] = _G(_englishExtraNouns)[i];

	_G(_fileLength) = f->size();

	_G(_game) = new GameInfo;

	Common::String md5 = g_vm->getGameMD5();
	const GlkDetectionEntry *p = SCOTT_GAMES;

	while (p->_md5) {
		if (md5.equalsC(p->_md5)) {
			if (!scumm_stricmp(p->_extra, "")) {
				CURRENT_GAME = SCOTTFREE;
			} else if (!scumm_stricmp(p->_extra, "ZXSpectrum")) {
				loadZXSpectrum(f, md5);
			} else if (!scumm_stricmp(p->_extra, "C64")) {
				loadC64(f, md5);
			}
			// TODO
			// TI99/4A Detection
		}
		++p;
	}

	if (CURRENT_GAME == SCOTTFREE || CURRENT_GAME == TI994A)
		return;

	/* Copy ZX Spectrum style system messages as base */
	for (int i = 6; i < MAX_SYSMESS && g_sysDictZX[i] != nullptr; i++) {
		_G(_sys)[i] = g_sysDictZX[i];
	}

	switch (CURRENT_GAME) {
	case HULK_C64:
	case HULK:
		for (int i = 0; i < 6; i++) {
			_G(_sys)[i] = g_sysDictZX[i];
		}
		break;
	default:
		if (!(_G(_game)->_subType & C64)) {
			if (_G(_game)->_subType & MYSTERIOUS) {
				for (int i = PLAY_AGAIN; i <= YOU_HAVENT_GOT_IT; i++)
					_G(_sys)[i] = _G(_systemMessages)[2 - PLAY_AGAIN + i];
				for (int i = YOU_DONT_SEE_IT; i <= WHAT_NOW; i++)
					_G(_sys)[i] = _G(_systemMessages)[15 - YOU_DONT_SEE_IT + i];
				for (int i = LIGHT_HAS_RUN_OUT; i <= RESUME_A_SAVED_GAME; i++)
					_G(_sys)[i] = _G(_systemMessages)[31 - LIGHT_HAS_RUN_OUT + i];
				_G(_sys)[ITEM_DELIMITER] = " - ";
				_G(_sys)[MESSAGE_DELIMITER] = "\n";
				_G(_sys)[YOU_SEE] = "\nThings I can see:\n";
				break;
			} else {
				for (int i = PLAY_AGAIN; i <= RESUME_A_SAVED_GAME; i++)
					_G(_sys)[i] = _G(_systemMessages)[2 - PLAY_AGAIN + i];
			}
		}
		break;
	}

	if (!(_G(_game)->_subType & (C64 | MYSTERIOUS))) {
		mysterious64Sysmess();
	}

	/* If it is a C64 or a Mysterious Adventures game, we have setup the graphics already */
	if (!(_G(_game)->_subType & (C64 | MYSTERIOUS)) && _G(_game)->_numberOfPictures > 0) {
		sagaSetup(0);
	}
}

} // End of namespace Scott
} // End of namespace Glk
