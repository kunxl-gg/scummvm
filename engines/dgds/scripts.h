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

#ifndef DGDS_SCRIPTS_H
#define DGDS_SCRIPTS_H

#include "common/rect.h"
#include "dgds/parser.h"
#include "dgds/scene.h"

namespace Dgds {

class DgdsEngine;
class DgdsChunkReader;


class ScriptParserData : public ParserData {
public:
	ScriptParserData() : scr(nullptr) {}
	Common::String filename;
	Common::SeekableReadStream *scr;
	Common::HashMap<uint16, Common::String> _tags;
};

class TTMData : public ScriptParserData {
public:
};

struct TTMState {
	TTMState() : scene(0), delay(0), _drawWin(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT), _currentBmpId(0) {}
	uint16 scene;
	int delay;
	Common::Rect _drawWin;
	int _currentBmpId;
	Common::String bmpNames[16];
};

class ADSData : public ScriptParserData {
public:
	ADSData() {}
	Common::Array<Common::String> names;
};

struct ADSState {
	ADSState() : scene(0), subIdx(0), subMax(0) {}
	uint16 scene;
	uint16 subIdx, subMax;
};

class TTMInterpreter {
public:
	TTMInterpreter(DgdsEngine *vm);

	bool load(const Common::String &filename);
	void unload();
	bool run();

	uint16 getScene() const { return _state.scene; }

protected:
	DgdsEngine *_vm;

	Dialogue _text;
	TTMData _scriptData;
	TTMState _state;
};

class ADSInterpreter {
public:
	ADSInterpreter(DgdsEngine *vm);
	~ADSInterpreter();

	bool load(const Common::String &filename);
	void unload();
	bool run();

protected:
	DgdsEngine *_vm;
	TTMInterpreter *_ttmInterpreter;

	ADSData _scriptData;
	ADSState _state;
};

} // End of namespace Dgds

#endif // DGDS_SCRIPTS_H
