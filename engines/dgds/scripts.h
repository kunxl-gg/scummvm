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
	TTMData() : _pages(330), ScriptParserData() {}
	uint16 _enviro;
	uint16 _pages;
	Common::Array<int> _pageOffsets;
	Common::String _bmpNames[16];
};

enum TTMRunType {
	kRunTypeStopped = 0,
	kRunType1 = 1,
	kRunTypeMulti = 2,
	kRunTypeTimeLimited = 3,
	kRunType4 = 4,
	kRunType5 = 5,
};


struct TTMSeq {
	TTMSeq() : _enviro(0), _seqNum(0), _currentFrame(0), _currentPalId(0), _startFrame(0) {
		reset();
	}

	void reset();

	int16 _enviro;
	int16 _seqNum;
	int16 _startFrame;
	int16 _gotoFrame;
	int16 _currentFrame;
	int16 _lastFrame;
	int16 _selfLoop;
	int16 _executed;
	int16 _slot[6];
	uint32 _timeNext;
	uint32 _timeCut;
	Common::Rect _drawWin;
	int _currentBmpId;
	int _currentPalId;
	byte _drawColFG;
	byte _drawColBG;
	int _runPlayed;
	int _runCount;
	TTMRunType _runFlag;
	int _scriptFlag;
};

class ADSData : public ScriptParserData {
public:
	ADSData() : _initFlag(0), _maxSegments(0) {
		for (int i = 0; i < ARRAYSIZE(_state); i++) {
			_state[i] = 8;
		}
		ARRAYCLEAR(_countdown);
		ARRAYCLEAR(_segments);
		ARRAYCLEAR(_charWhile);
	}
	Common::Array<Common::String> _scriptNames;
	Common::Array<TTMData> _scriptEnvs;
	Common::Array<TTMSeq> _ttmSeqs;
	int _initFlag;
	int _maxSegments;
	// TODO: replace these with dynamic arrays - fixed arrays inherited from original.
	int _state[80];
	int _countdown[80];
	// note: originals uses char * but we use offsets into script for less pointers..
	int32 _segments[80];
	int32 _charWhile[80];
	Common::Array<struct TTMSeq *> _usedSeqs[80];
};

class TTMInterpreter {
public:
	TTMInterpreter(DgdsEngine *vm);

	bool load(const Common::String &filename, TTMData &env);
	void unload();
	bool run(TTMData &env, struct TTMSeq &seq);
	void findAndAddSequences(TTMData &scriptData, Common::Array<TTMSeq> &seqArray);

protected:
	void handleOperation(TTMData &env, struct TTMSeq &seq, uint16 op, byte count, const int16 *ivals, const Common::String &sval);
	void updateScreen(struct TTMSeq &seq);

	DgdsEngine *_vm;
};

class ADSInterpreter {
public:
	ADSInterpreter(DgdsEngine *vm);
	~ADSInterpreter();

	bool load(const Common::String &filename);
	void unload();
	bool run();
	int numArgs(uint16 opcode) const;
	void segmentOrState(int16 seg, uint16 val);
	void segmentSetState(int16 seg, uint16 val);

protected:
	bool handleOperation(uint16 code, Common::SeekableReadStream *scr);
	bool playScene();
	void skipToEndIf(Common::SeekableReadStream *scr);
	TTMSeq *findTTMSeq(int16 enviro, int16 seq);
	TTMData *findTTMEnviro(int16 enviro);
	bool runUntilScenePlayedOrEnd();
	void findUsedSequencesForSegment(int segno);
	void findEndOrInitOp();

	DgdsEngine *_vm;
	TTMInterpreter *_ttmInterpreter;

	ADSData _adsData;

	TTMSeq *_currentTTMSeq;
};

} // End of namespace Dgds

#endif // DGDS_SCRIPTS_H
