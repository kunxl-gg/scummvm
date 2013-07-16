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
 */

#ifndef ZVISION_ZORK_RAW_H
#define ZVISION_ZORK_RAW_H

#include "common/types.h"
#include "audio/audiostream.h"

namespace Common {
class SeekableReadStream;
}

namespace ZVision {

class ZVision;

struct SoundParams {
	char identifier;
	uint16 rate;
	bool stereo;
	bool packed;
};

const SoundParams zNemSoundParamLookupTable[6] = {{'6', 0x2B11, false, false},
                                                  {'a', 0x5622, false,  true}, 
                                                  {'b', 0x5622, true,  true},
                                                  {'n', 0x2B11, false, true},
                                                  {'s', 0x5622, false, true},
                                                  {'t', 0x5622, true, true}
};

const SoundParams zgiSoundParamLookupTable[5] = {{'a',0x5622, false, false},
                                                 {'k',0x2B11, true, true},
                                                 {'p',0x5622, false, true},
                                                 {'q',0x5622, true, true},
                                                 {'u',0xAC44, true, true}
};

/**
 * This is a stream, which allows for playing raw ADPCM data from a stream.
 */
class RawZorkStream : public Audio::RewindableAudioStream {
public:
	RawZorkStream(uint32 rate, bool stereo, bool packed, DisposeAfterUse::Flag disposeStream, Common::SeekableReadStream *stream);
	RawZorkStream(const Common::String &filePath, ZVision *engine);

	~RawZorkStream() {
	}

public:

private:
	const int _rate;                                           // Sample rate of stream
	Audio::Timestamp _playtime;                                // Calculated total play time
	Common::DisposablePtr<Common::SeekableReadStream> _stream; // Stream to read data from
	bool _endOfData;                                           // Whether the stream end has been reached
	bool _stereo;
	bool _packed;

	/** 
	 * Holds the frequency and index from the last sample
	 * 0 holds the left channel, 1 holds the right channel
	 */
	struct {
		int32 sample;
		int16 index;
	} _lastSample[2];

	static const int16 _stepAdjustmentTable[8];
	static const int32 _amplitudeLookupTable[89];

public:
	int readBuffer(int16 *buffer, const int numSamples);

	bool isStereo() const { return true; }
	bool endOfData() const { return _endOfData; }

	int getRate() const { return _rate; }
	Audio::Timestamp getLength() const { return _playtime; }

	bool rewind();

private:
	int decodeADPCM(int16 *buffer, const int numSamples);
};

/**
 * Creates an audio stream, which plays from the given buffer.
 *
 * @param buffer Buffer to play from.
 * @param size   Size of the buffer in bytes.
 * @param rate   Rate of the sound data.
 * @param disposeAfterUse Whether to free the buffer after use (with free!).
 * @return The new SeekableAudioStream (or 0 on failure).
 */
Audio::RewindableAudioStream *makeRawZorkStream(const byte *buffer, uint32 size,
                                                int rate,
								                bool stereo,
												bool packed,
                                                DisposeAfterUse::Flag disposeAfterUse = DisposeAfterUse::YES);

/**
 * Creates an audio stream, which plays from the given stream.
 *
 * @param stream Stream object to play from.
 * @param rate   Rate of the sound data.
 * @param disposeAfterUse Whether to delete the stream after use.
 * @return The new SeekableAudioStream (or 0 on failure).
 */
Audio::RewindableAudioStream *makeRawZorkStream(Common::SeekableReadStream *stream,
                                                int rate,
								                bool stereo,
												bool packed,
                                                DisposeAfterUse::Flag disposeAfterUse = DisposeAfterUse::YES);

Audio::RewindableAudioStream *makeRawZorkStream(const Common::String &filePath, ZVision *engine);

} // End of namespace ZVision

#endif
