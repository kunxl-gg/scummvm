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

/*
 * This code is based on the CRAB engine
 *
 * Copyright (c) Arvind Raja Yadav
 *
 * Licensed under MIT
 *
 */

#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include "crab/level/level.h"

namespace Crab {

using namespace TMX;
using namespace pyrodactyl::stat;
using namespace pyrodactyl::anim;
using namespace pyrodactyl::level;
using namespace pyrodactyl::image;
using namespace pyrodactyl::people;
using namespace pyrodactyl::input;


// Purpose: Pre render the terrain layer
void Level::PreDraw() {
	Graphics::ManagedSurface *surf = new Graphics::ManagedSurface(terrain.W(), terrain.H(), *g_engine->_format);
	unsigned int layer_count = 0u;
	for (auto l = terrain.layer.begin(); l != terrain.layer.end(); ++l, ++layer_count) {
		g_engine->_imageManager->tileset.PreDraw(*l, terrain.tile_size, surf);

		// Draw the normal sprites if this is the layer for it
		if (layer_count == terrain.sprite_layer)
			PreDrawObjects(surf);
	}

	// This is to ensure we don't miss out on drawing sprites
	if (layer_count <= terrain.sprite_layer)
		PreDrawObjects(surf);

	img.Load(surf);
}

//------------------------------------------------------------------------
// Purpose: Draw the level
//------------------------------------------------------------------------
void Level::Draw(pyrodactyl::event::Info &info) {
	SetCamera();
	SortObjectsToDraw();

	// Background sprites are assumed to be sorted by their layer count
	auto b = background.begin();

	unsigned int layer_count = 0u;

	// Draw the terrain layer
	g_engine->_imageManager->tileset.Draw(terrain.layer[0], camera, terrain.tile_size, objects[player_index].PosRect(), img);

	Vector2i pos = objects[player_index].ai_data.dest;
	Rect newpos(pos.x - (dest_marker.size.x/2), pos.y - (dest_marker.size.y/2), dest_marker.size.x, dest_marker.size.y);

	for (auto l = terrain.layer.begin(); l != terrain.layer.end(); ++l, ++layer_count) {

		// Draw the background sprites
		for (; b != background.end(); ++b) {
			if ((unsigned int)b->layer > layer_count) // We don't have any sprites to draw at this layer
				break;
			else if ((unsigned int)b->layer == layer_count && b->Visible())
				b->Draw(info, camera);
		}

		// Draw the normal sprites if this is the layer for it
		if (layer_count == terrain.sprite_layer)
			DrawObjects(info);

		// Draw the terrain layer
		g_engine->_imageManager->tileset.ForceDraw(*l, camera, terrain.tile_size, objects[player_index].PosRect());
	}

	// Draw the background sprites
	for (; b != background.end(); ++b) {
		if ((unsigned int)b->layer > layer_count) // We don't have any sprites to draw at this layer
			break;
		else if ((unsigned int)b->layer >= layer_count && b->Visible())
			b->Draw(info, camera);
	}

	// This is to ensure we don't miss out on drawing sprites
	if (layer_count <= terrain.sprite_layer)
		DrawObjects(info);

	// Fliers are drawn above every sprite but below popup text
	for (auto &i : fly) {
		// Only draw if it is supposed to be flying
		if (i.ai_data.walk.enabled)
			i.Draw(info, camera);
	}

	// Draw popup text over all level layers
	for (auto &i : objects) {
		// Only draw popups for visible sprites
		if (i.Visible()) {
			if (i.PopupShow())
				i.DrawPopup(pop, camera);
			else {
				// Only draw their name if they are
				//(a) hovered over by the mouse, OR
				//(b) are in talk range and don't have popup text over their head
				if (i.hover || (info.LastPerson() == i.ID() && !i.PopupShow()))
					talk_notify.Draw(info, i, camera);
			}
		}
	}

	if (GameDebug)
		terrain.DrawDebug(camera);
}

//------------------------------------------------------------------------
// Purpose: Draw sprites and prop layers in sequences according to depth
//------------------------------------------------------------------------
void Level::DrawObjects(pyrodactyl::event::Info &info) {
	// Draw player destination marker
	if (objects[player_index].ai_data.dest.active)
		dest_marker.Draw(objects[player_index].ai_data.dest, camera);

	Vector2i pos = objects[player_index].ai_data.dest;
	Rect newpos(pos.x - (dest_marker.size.x/2), pos.y - (dest_marker.size.y/2), dest_marker.size.x, dest_marker.size.y);

	if (terrain.prop.empty()) {
		for (auto &entry : obj_seq) {
			if (entry.second->Visible() && LayerVisible(entry.second))
				entry.second->Draw(info, camera);
		}
	} else {
		auto a = terrain.prop.begin();
		auto b = obj_seq.begin();

		while (a != terrain.prop.end() && b != obj_seq.end()) {
			auto obj = b->second;
			if (a->pos.y + a->pos.h < obj->Y() + obj->H()) {
				for (auto &i : a->boundRect) {
					i.Draw(-camera.x, -camera.y, 128, 128, 0, 255);
					if (i.Collide(objects[player_index].PosRect())) {
						g_engine->_imageManager->tileset.ForceDraw(*a, camera, terrain.tile_size, objects[player_index].PosRect());
					}

					if (i.Contains(objects[player_index].ai_data.dest)) {
						g_engine->_imageManager->tileset.ForceDraw(*a, camera, terrain.tile_size, newpos);
					}
				}
				++a;
			} else {
				if (obj->Visible() && LayerVisible(obj))
					obj->Draw(info, camera);
				++b;
			}
		}

		if (a == terrain.prop.end()) {
			for (; b != obj_seq.end(); ++b) {
				auto obj = b->second;
				if (obj->Visible() && LayerVisible(obj))
					obj->Draw(info, camera);
			}
		} else if (b == obj_seq.end()) {
			for (; a != terrain.prop.end(); ++a) {
				for (auto &i : a->boundRect) {
					i.Draw(-camera.x, -camera.y, 128, 128, 0, 255);
					if (i.Collide(objects[player_index].PosRect())) {
						g_engine->_imageManager->tileset.ForceDraw(*a, camera, terrain.tile_size, objects[player_index].PosRect());
					}

					if (i.Contains(objects[player_index].ai_data.dest)) {
						g_engine->_imageManager->tileset.ForceDraw(*a, camera, terrain.tile_size, newpos);
					}
				}
			}
		}
	}
}

void Level::PreDrawObjects(Graphics::ManagedSurface *surf) {
	if (terrain.prop.empty()) {
		return;
	} else {
		auto a = terrain.prop.begin();
		auto b = obj_seq.begin();

		while (a != terrain.prop.end() && b != obj_seq.end()) {
			auto obj = b->second;
			if (a->pos.y + a->pos.h < obj->Y() + obj->H()) {
				g_engine->_imageManager->tileset.PreDraw(*a, terrain.tile_size, surf);
				++a;
			} else {
				++b;
			}
		}

		if (a == terrain.prop.end()) {
		} else if (b == obj_seq.end()) {
			for (; a != terrain.prop.end(); ++a)
				g_engine->_imageManager->tileset.PreDraw(*a, terrain.tile_size, surf);
		}
	}
}

//------------------------------------------------------------------------
// Purpose: Sort objects to draw them according to their Y coordinates
//------------------------------------------------------------------------
bool Level::operator()(int i, int j) {
	return objects[i].Y() + objects[i].H() < objects[j].Y() + objects[j].H();
}

void Level::SortObjectsToDraw() {
	// add each object to the map to sort it
	obj_seq.clear();
	for (auto &object : objects)
		obj_seq.insert(Common::Pair<int, Sprite *>(object.Y() + object.H(), &object));
}

//------------------------------------------------------------------------
// Purpose: Center the camera on the player for scrolling levels
//------------------------------------------------------------------------
void Level::SetCamera() {
	// Use the focus points of sprites
	Vector2i focus = objects[player_index].CamFocus();

	// Center the camera over the player
	camera.x = focus.x - (g_engine->_screenSettings->cur.w / 2);
	camera.y = focus.y - (g_engine->_screenSettings->cur.h / 2);

	// Keep the camera in bounds
	if (camera.x > terrain.W() - camera.w)
		camera.x = terrain.W() - camera.w;
	if (camera.y > terrain.H() - camera.h)
		camera.y = terrain.H() - camera.h;
	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
}

} // End of namespace Crab
