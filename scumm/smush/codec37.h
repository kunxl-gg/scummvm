/* ScummVM - Scumm Interpreter
 * Copyright (C) 2001/2002 The ScummVM project
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Header$
 *
 */

#ifndef CODEC37_H
#define CODEC37_H

#include "config.h"

#ifdef DEBUG
# ifndef NO_DEBUG_CODEC37
#  define DEBUG_CODEC37
# endif
#else
# ifdef DEBUG_CODEC37
#  error DEBUG_CODEC37 defined without DEBUG
# endif
#endif

#ifdef DEBUG_CODEC37
# ifndef NO_DEBUG_CODEC37_PROCS
#  define DEBUG_CODEC37_PROC1
#  define DEBUG_CODEC37_PROC2
#  define DEBUG_CODEC37_PROC3
#  define DEBUG_CODEC37_PROC4
# endif
#endif

#include "decoder.h"

/*!	@brief ::decoder for codec 37.

*/

#define DELTA_ADD 0x3E00	// what is this 0x3E00 ?? == 320*200/4 - 128
				// It looks like it is a safe-guarding protection from bugs., but maybe not...

class Codec37Decoder : public Decoder {
private:
	int32 _deltaSize;
	byte * _deltaBufs[2];
	byte * _deltaBuf;
	int16 * _offsetTable;
	int32 _curtable;
	uint16 _prevSeqNb;
	int32 _tableLastPitch;
	int32 _tableLastIndex;

public:
	bool initSize(const Point &, const Rect &);
	Codec37Decoder();
	void clean();
	virtual ~Codec37Decoder();
protected:
	static inline uint32 expand(byte b) {
		uint32 r = b | (b << 8);
		return r | (r << 16);
	}
	void maketable(int32, int32);
	void proc1(Blitter &, Chunk &, int32, int32, int32, int32);
	void proc2(Blitter &, Chunk &, int32);
	void proc3WithFDFE(Blitter &, Chunk &, int32, int32, int32);
	void proc3WithoutFDFE(Blitter &, Chunk &, int32, int32, int32);
	void proc4WithFDFE(Blitter &, Chunk &, int32, int32, int32);
	void proc4WithoutFDFE(Blitter &, Chunk &, int32, int32, int32);
public:
	bool decode(Blitter &, Chunk &);
};

#endif
