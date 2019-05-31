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

#include "common/scummsys.h"
#include "common/system.h"
#include "common/config-manager.h"
#include "common/debug.h"
#include "common/debug-channels.h"
#include "common/file.h"
#include "common/error.h"
#include "graphics/surface.h"

#include "hdb.h"
#include "console.h"

namespace HDB {

HDBGame::HDBGame(OSystem *syst, const ADGameDescription *gameDesc) : Engine(syst), _gameDescription(gameDesc) {
	_console = nullptr;

	DebugMan.addDebugChannel(kDebugExample1, "Example1", "This is just an example to test");
	DebugMan.addDebugChannel(kDebugExample2, "Example2", "This is also an example");
}

HDBGame::~HDBGame() {
	delete _console;
	DebugMan.clearAllDebugChannels();
}

bool HDBGame::init() {
	_voiceless = false;

	/*
		Game Subsystem Initializations
	*/

	// Init _fileMan
	if (fileMan->openMPC("hyperdemo.mpc")) {
		gameShutdown = false;
		return true;
	}

	error("FileMan::openMPC: Cannot find the hyperspace.mpc data file.");
	return false;
}

void HDBGame::start() {
	_gameState = GameState::GAME_TITLE;
}

/*
	Changes the current GameState to the next one.
	Game State Transitions are deterministic: each state can
	only a particular state. The next state is held in gameState.

	TODO: All the functionality hasn't been implemented yet since
	their subsystems are incomplete. This section needs to be periodically
	updated as soon as the subsytems are improved.
*/
void HDBGame::changeGameState() {

	switch (_gameState) {
	case GameState::GAME_TITLE:
		_gameState = GameState::GAME_MENU;
		break;
	case GameState::GAME_MENU:
		_gameState = GameState::GAME_PLAY;
		break;
	case GameState::GAME_PLAY:
		_gameState = GameState::GAME_MENU;
		break;
	}
}

Common::Error HDBGame::run() {
	// Initializes Graphics
	Graphics::PixelFormat format(4, 8, 8, 8, 8, 24, 16, 8, 0);
	initGraphics(640, 320, &format);
	_console = new Console();

	/*
	if (!_game->init()) {
		error("Couldn't initialize Game.");
		return Common::kUnknownError;
	}

	_game->start();
	
	_game->mainLoop();
	*/
	
	while (!shouldQuit()) {

		Common::Event event;
		while (g_system->getEventManager()->pollEvent(event)) {
			switch (event.type) {
			case Common::EVENT_QUIT:
			case Common::EVENT_RTL:
				break;
			case Common::EVENT_KEYDOWN:
				debug("Key was pressed.");
				break;
			default:
				break;
			}
		}
		
		Graphics::Surface screen;
		screen.create(800, 600, g_system->getScreenFormat());
		screen.drawLine(100, 100, 200, 200, 0xffffffff);
		
		g_system->copyRectToScreen(screen.getBasePtr(0, 0), screen.pitch, 0, 0, 800, 600);
		
		g_system->updateScreen();
		g_system->delayMillis(10);
	}

	return Common::kNoError;
}

} // End of namespace HDB
