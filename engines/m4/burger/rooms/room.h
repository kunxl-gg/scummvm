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

#ifndef M4_BURGER_ROOMS_ROOM_H
#define M4_BURGER_ROOMS_ROOM_H

#include "m4/core/rooms.h"
#include "m4/burger/core/play_break.h"
#include "m4/burger/core/stream_break.h"
#include "m4/burger/core/conv.h"
#include "m4/adv_r/conv_io.h"
#include "m4/core/imath.h"
#include "m4/graphics/gr_series.h"
#include "m4/adv_r/adv_hotspot.h"

namespace M4 {
namespace Burger {
namespace Rooms {

class Room : public M4::Room {
public:
	Room() : M4::Room() {}
	~Room() override {}

	void shutdown() override;
	void parser_code() override;

	void compact_mem_and_report() {}
};

} // namespace Rooms
} // namespace Burger
} // namespace M4

#endif
