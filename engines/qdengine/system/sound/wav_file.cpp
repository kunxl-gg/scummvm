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

#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include "audio/decoders/wave.h"
#include "audio/mixer.h"
#include "common/system.h"
#include "qdengine/qdengine.h"
#include "qdengine/qd_precomp.h"
#include "qdengine/system/sound/wav_file.h"
#include "qdengine/system/sound/wav_sound.h"
#include "qdengine/qdcore/qd_file_manager.h"


namespace QDEngine {

bool wav_file_load(const char *fname, class wavSound *snd) {
	if (!fname) {
		return false;
	}

	Common::Path fpath(fname, '\\');
	Common::SeekableReadStream *stream;

	if (qdFileManager::instance().open_file(&stream, fpath.toString().c_str(), false)) {
		Audio::SeekableAudioStream *audioStream = Audio::makeWAVStream(stream, DisposeAfterUse::YES);
		snd->_audiostream = audioStream;
	}

	warning("STUB: wav_file_load() %s", transCyrillic(fname));
	return true;
}

} // namespace QDEngine
