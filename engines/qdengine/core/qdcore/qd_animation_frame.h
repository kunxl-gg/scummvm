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

#ifndef QDENGINE_CORE_QDCORE_QD_ANIMATION_FRAME_H
#define QDENGINE_CORE_QDCORE_QD_ANIMATION_FRAME_H

#include "qdengine/core/qdcore/qd_sprite.h"


namespace QDEngine {

//! Кадр анимации.
class qdAnimationFrame : public qdSprite {
public:
	qdAnimationFrame();
	qdAnimationFrame(const qdAnimationFrame &frm);
	~qdAnimationFrame();

	qdAnimationFrame &operator = (const qdAnimationFrame &frm);

	qdAnimationFrame *clone() const;

	float start_time() const {
		return start_time_;
	}
	float end_time() const {
		return start_time_ + length_;
	}
	float length() const {
		return length_;
	}

	void set_start_time(float tm) {
		start_time_ = tm;
	}
	void set_length(float tm) {
		length_ = tm;
	}

	virtual void qda_load(class XStream &fh, int version = 100);
	virtual void qda_load(class XZipStream &fh, int version = 100);
	virtual void qda_load(class Common::SeekableReadStream *fh, int version = 100);

	bool load_resources();
	void free_resources();

private:
	float start_time_;
	float length_;
};

typedef std::list<qdAnimationFrame *> qdAnimationFrameList;

} // namespace QDEngine

#endif // QDENGINE_CORE_QDCORE_QD_ANIMATION_FRAME_H
