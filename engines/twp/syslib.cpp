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

#include "twp/sqgame.h"
#include "twp/twp.h"
#include "twp/ids.h"
#include "twp/squtil.h"
#include "twp/thread.h"
#include "twp/task.h"
#include "twp/callback.h"
#include "twp/scenegraph.h"
#include "twp/squirrel/squirrel.h"
#include "twp/squirrel/sqvm.h"
#include "twp/squirrel/sqobject.h"
#include "twp/squirrel/sqstring.h"
#include "twp/squirrel/sqstate.h"
#include "twp/squirrel/sqtable.h"
#include "twp/squirrel/sqstdstring.h"
#include "twp/squirrel/sqstdmath.h"
#include "twp/squirrel/sqstdio.h"
#include "twp/squirrel/sqstdaux.h"
#include "twp/squirrel/sqfuncproto.h"
#include "twp/squirrel/sqclosure.h"
#include "common/debug.h"

namespace Twp {

static SQInteger _startthread(HSQUIRRELVM v, bool global) {
	HSQUIRRELVM vm = g_engine->getVm();
	SQInteger size = sq_gettop(v);
	HSQOBJECT envObj, threadObj, closureObj;

	sq_resetobject(&envObj);
	if (SQ_FAILED(sq_getstackobj(v, 1, &envObj)))
		return sq_throwerror(v, "Couldn't get environment from stack");

	// create thread and store it on the stack
	sq_newthread(vm, 1024);
	sq_resetobject(&threadObj);
	if (SQ_FAILED(sq_getstackobj(vm, -1, &threadObj)))
		return sq_throwerror(v, "Couldn't get coroutine thread from stack");

	Common::Array<HSQOBJECT> args;
	for (int i = 0; i < size - 2; i++) {
		HSQOBJECT arg;
		sq_resetobject(&arg);
		if (SQ_FAILED(sq_getstackobj(v, 3 + i, &arg)))
			return sq_throwerror(v, "Couldn't get coroutine args from stack");
		args.push_back(arg);
	}

	// get the closure
	sq_resetobject(&closureObj);
	if (SQ_FAILED(sq_getstackobj(v, 2, &closureObj)))
		return sq_throwerror(v, "Couldn't get coroutine thread from stack");

	const SQChar *name = nullptr;
	if (SQ_SUCCEEDED(sq_getclosurename(v, 2)))
		sq_getstring(v, -1, &name);

	Common::String threadName = Common::String::format("%s %s (%lld)", name == nullptr ? "<anonymous>" : name, _stringval(_closure(closureObj)->_function->_sourcename), _closure(closureObj)->_function->_lineinfos->_line);
	Thread *t = new Thread(threadName, global, threadObj, envObj, closureObj, args);
	sq_pop(vm, 1);
	if (name)
		sq_pop(v, 1); // pop name
	sq_pop(v, 1);     // pop closure

	g_engine->_threads.push_back(t);

	debug("create thread %s", t->getName().c_str());

	// call the closure in the thread
	if (!t->call())
		return sq_throwerror(v, "call failed");

	sqpush(v, t->getId());
	return 1;
}

static SQInteger breakfunc(HSQUIRRELVM v, void func(ThreadBase *t, void *data), void *data) {
	ThreadBase *t = sqthread(v);
	if (!t)
		return sq_throwerror(v, "failed to get thread");
	t->suspend();
	func(t, data);
	return -666;
}

static SQInteger activeController(HSQUIRRELVM v) {
	warning("TODO: activeController: not implemented");
	// TODO: change harcoded mouse
	sq_pushinteger(v, 1);
	return 1;
}

// Sets a timer of duration seconds.
//
// When the timer is up, method will be executed.
// Use this method so that the callback will get saved.
// That is, if you set a callback to call method in 30 minutes, play the game for 10 minutes, save and quit;
// when you return to the game, it will remember that it needs to wait 20 minutes before calling method.
// If the game is paused, all callback timers are paused.
// Note, method cannot be code, it must be a defined script or function (otherwise, the game wouldn't be able to save what it needs to do when the timer is up).
// .. code-block:: Squirrel
// if (actorTalking()) {
//   addCallback(30, doADance)    // Wait another 30 seconds
//   return
//}
static SQInteger addCallback(HSQUIRRELVM v) {
	SQInteger count = sq_gettop(v);
	float duration;
	if (SQ_FAILED(sqget(v, 2, duration)))
		return sq_throwerror(v, "failed to get duration");
	HSQOBJECT meth;
	sq_resetobject(&meth);
	if (SQ_FAILED(sq_getstackobj(v, 3, &meth)) || !sq_isclosure(meth))
		return sq_throwerror(v, "failed to get method");

	Common::String methodName;
	if (SQ_SUCCEEDED(sq_getclosurename(v, 3))) {
		const SQChar *tmpMethodName;
		sq_getstring(v, -1, &tmpMethodName);
		methodName = tmpMethodName;
	}

	Common::Array<HSQOBJECT> args;
	for (int i = 4; i <= count; i++) {
		HSQOBJECT arg;
		sq_resetobject(&arg);
		if (SQ_FAILED(sq_getstackobj(v, i, &arg)))
			return sq_throwerror(v, Common::String::format("failed to get argument %d", i).c_str());
		args.push_back(arg);
	}

	Callback *callback = new Callback(newCallbackId(), duration, methodName, args);
	g_engine->_callbacks.push_back(callback);

	sqpush(v, callback->getId());
	return 1;
}

// Registers a folder that assets can appear in.
//
// Only used for development builds where the assets are not bundled up.
// Use in the Boot.nut process.
// Not necessary for release.
static SQInteger addFolder(HSQUIRRELVM v) {
	return 0;
}

static void threadFrames(ThreadBase *tb, void *data) {
	int numFrames = *(int *)data;
	((Thread *)tb)->_numFrames = numFrames;
}

static void threadTime(ThreadBase *tb, void *data) {
	float time = *(float *)data;
	((Thread *)tb)->_waitTime = time;
}

// When called in a function started with startthread, execution is suspended for count frames.
// It is an error to call breakhere in a function that was not started with startthread.
// Particularly useful instead of breaktime if you just want to wait 1 frame, since not all machines run at the same speed.
// . code-block:: Squirrel
// while(isSoundPlaying(soundPhoneBusy)) {
//   breakhere(5)
//}
static SQInteger breakhere(HSQUIRRELVM v) {
	SQObjectType t = sq_gettype(v, 2);
	if (t == OT_INTEGER) {
		int numFrames;
		if (SQ_FAILED(sqget(v, 2, numFrames)))
			return sq_throwerror(v, "failed to get numFrames");
		return breakfunc(v, threadFrames, &numFrames);
	}
	if (t == OT_FLOAT) {
		float time;
		if (SQ_FAILED(sqget(v, 2, time)))
			return sq_throwerror(v, "failed to get time");
		return breakfunc(v, threadTime, &time);
	}
	return sq_throwerror(v, Common::String::format("failed to get numFrames (wrong type = {%d})", t).c_str());
}

// When called in a function started with startthread, execution is suspended for time seconds.
// It is an error to call breaktime in a function that was not started with startthread.
//
// . code-block:: Squirrel
// for (local x = 1; x < 4; x += 1) {
//   playSound(soundPhoneRinging)
//   breaktime(5.0)
// }
static SQInteger breaktime(HSQUIRRELVM v) {
	SQFloat time;
	if (SQ_FAILED(sq_getfloat(v, 2, &time)))
		return sq_throwerror(v, "failed to get time");
	if (time == 0.f) {
		int frame = 1;
		return breakfunc(v, threadFrames, &frame);
	}
	return breakfunc(v, threadTime, &time);
}

template<typename Predicate>
static SQInteger breakwhilecond(HSQUIRRELVM v, Predicate pred, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	Common::String name = Common::String::format(fmt, va);
	va_end(va);

	ThreadBase *curThread = sqthread(v);
	if (!curThread)
		return sq_throwerror(v, "Current thread should be created with startthread");

	debug("add breakwhilecond name=%s pid=%d, %s", name.c_str(), curThread->getId(), curThread->getName().c_str());
	g_engine->_tasks.push_back(new BreakWhileCond<Predicate>(curThread->getId(), name, pred));
	return -666;
}

static bool isAnimating(Object *obj) {
	return obj->_nodeAnim->_anim && !obj->_nodeAnim->_disabled && obj->_animName != obj->getAnimName(STAND_ANIMNAME);
}

struct ObjAnimating {
	ObjAnimating(Object *obj) : _obj(obj) {}
	bool operator()() {
		return isAnimating(_obj);
	}

private:
	Object *_obj;
};

// When called in a function started with startthread, execution is suspended until animatingItem has completed its animation.
// Note, animatingItem can be an actor or an object.
// It is an error to call breakwhileanimating in a function that was not started with `startthread`.
//
// . code-block:: Squirrel
// actorFace(ray, FACE_LEFT)
// actorCostume(ray, "RayVomit")
// actorPlayAnimation(ray, "vomit")
// breakwhileanimating(ray)
// actorCostume(ray, "RayAnimation")
static SQInteger breakwhileanimating(HSQUIRRELVM v) {
	Object *obj = sqobj(v, 2);
	if (!obj)
		return sq_throwerror(v, "failed to get object");
	return breakwhilecond(v, ObjAnimating(obj), "breakwhileanimating(%s)", obj->_key.c_str());
}

struct CameraMoving {
	bool operator()() {
		return g_engine->_camera.isMoving();
	}
};

// Breaks while a camera is moving.
// Once the thread finishes execution, the method will continue running.
// It is an error to call breakwhilecamera in a function that was not started with startthread.
static SQInteger breakwhilecamera(HSQUIRRELVM v) {
	return breakwhilecond(v, CameraMoving(), "breakwhilecamera()");
}

struct CutsceneRunning {
	bool operator()() {
		return g_engine->_cutscene != nullptr;
	}
};

// Breaks while a cutscene is running.
// Once the thread finishes execution, the method will continue running.
// It is an error to call breakwhilecutscene in a function that was not started with startthread.
static SQInteger breakwhilecutscene(HSQUIRRELVM v) {
	return breakwhilecond(v, CutsceneRunning(), "breakwhilecutscene()");
}

struct DialogRunning {
	bool operator()() {
		return g_engine->_dialog.getState() != DialogState::None;
	}
};

// Breaks while a dialog is running.
// Once the thread finishes execution, the method will continue running.
// It is an error to call breakwhiledialog in a function that was not started with startthread.
static SQInteger breakwhiledialog(HSQUIRRELVM v) {
	return breakwhilecond(v, DialogRunning(), "breakwhiledialog()");
}

struct InputOff {
	bool operator()() {
		return !g_engine->_inputState.getInputActive();
	}
};

// Breaks while input is not active.
// Once the thread finishes execution, the method will continue running.
// It is an error to call breakwhileinputoff in a function that was not started with startthread.
static SQInteger breakwhileinputoff(HSQUIRRELVM v) {
	return breakwhilecond(v, InputOff(), "breakwhileinputoff()");
}

// Breaks while the thread referenced by threadId is running.
// Once the thread finishes execution, the method will continue running.
// It is an error to call breakwhilerunning in a function that was not started with startthread.
//
// . code-block:: Squirrel
// local waitTID = 0
//
//    if ( g.in_flashback && HotelElevator.requestedFloor == 13 ) {
//     waitTID = startthread(HotelElevator.avoidPenthouse)
//     breakwhilerunning(waitTID)
// }
// waitTID = 0
// if (HotelElevator.requestedFloor >= 0) {
//     // Continue executing other code
// }
static SQInteger breakwhilerunning(HSQUIRRELVM v) {
	int id = 0;
	if (sq_gettype(v, 2) == OT_INTEGER)
		sqget(v, 2, id);
	debug("breakwhilerunning: %d", id);

	ThreadBase *t = sqthread(id);
	if (!t) {
		if (!isSound(id)) {
			warning("thread and sound not found: %d", id);
			return 0;
		}
		return breakwhilecond(
			v, [id] { return g_engine->_audio.playing(id); }, "breakwhilerunningsound(%d)", id);
	}
	return breakwhilecond(
		v, [id] { return sqthread(id) != nullptr; }, "breakwhilerunning(%d)", id);
}

// Returns true if at least 1 actor is talking.
static bool isSomeoneTalking() {
	for (auto it = g_engine->_actors.begin(); it != g_engine->_actors.end(); it++) {
		Object *obj = *it;
		if (obj->getTalking() && obj->getTalking()->isEnabled())
			return true;
	}
	for (auto it = g_engine->_room->_layers.begin(); it != g_engine->_room->_layers.end(); it++) {
		Layer *layer = *it;
		for (auto it2 = layer->_objects.begin(); it2 != layer->_objects.end(); it2++) {
			Object *obj = *it2;
			if (obj->getTalking() && obj->getTalking()->isEnabled())
				return true;
		}
	}
	return false;
}

struct SomeoneTalking {
	bool operator()() {
		return isSomeoneTalking();
	}
};

struct ActorTalking {
	ActorTalking(Object *obj) : _obj(obj) {}

	bool operator()() {
		return _obj->getTalking() && _obj->getTalking()->isEnabled();
	}

private:
	Object *_obj;
};

// If an actor is specified, breaks until actor has finished talking.
// If no actor is specified, breaks until ALL actors have finished talking.
// Once talking finishes, the method will continue running.
// It is an error to call breakwhiletalking in a function that was not started with startthread.
//
// . code-block:: Squirrel
// while(closeToWillie()) {
//     local line = randomfrom(lines)
//     breakwhiletalking(willie)
//     mumbleLine(willie, line)
//     breakwhiletalking(willie)
// }
static SQInteger breakwhiletalking(HSQUIRRELVM v) {
	SQInteger nArgs = sq_gettop(v);
	if (nArgs == 1) {
		return breakwhilecond(v, SomeoneTalking(), "breakwhiletalking(all)");
	}
	if (nArgs == 2) {
		Object *obj = sqobj(v, 2);
		if (!obj)
			return sq_throwerror(v, "failed to get object");
		return breakwhilecond(v, ActorTalking(obj), "breakwhiletalking(%s)", obj->_name.c_str());
	}

	return sq_throwerror(v, "Invalid number of arguments for breakwhiletalking");
}

struct ActorWalking {
	ActorWalking(Object *obj) : _obj(obj) {}

	bool operator()() {
		return _obj->getWalkTo() && _obj->getWalkTo()->isEnabled();
	}

private:
	Object *_obj;
};

// If an actor is specified, breaks until actor has finished walking.
// Once arrived at destination, the method will continue running.
// It is an error to call breakwhilewalking in a function that was not started with `startthread`.
//
// . code-block:: Squirrel
// startthread(@(){
//    actorWalkTo(currentActor, Nickel.copyTron)
//    breakwhilewalking(currentActor)
//    pushSentence(VERB_USE, nickel, Nickel.copyTron)
//})
static SQInteger breakwhilewalking(HSQUIRRELVM v) {
	Object *obj = sqobj(v, 2);
	if (!obj)
		return sq_throwerror(v, "failed to get object");
	return breakwhilecond(v, ActorWalking(obj), "breakwhilewalking(%s)", obj->_name.c_str());
}

struct SoundPlaying {
	SoundPlaying(int soundId) : _soundId(soundId) {}

	bool operator()() {
		return g_engine->_audio.playing(_soundId);
	}

private:
	int _soundId;
};

// Breaks until specified sound has finished playing.
// Once sound finishes, the method will continue running.
static SQInteger breakwhilesound(HSQUIRRELVM v) {
	int soundId = 0;
	if (SQ_FAILED(sqget(v, 2, soundId)))
		return sq_throwerror(v, "failed to get sound");
	return breakwhilecond(v, SoundPlaying(soundId), "breakwhilesound(%d)", soundId);
}

static SQInteger cutscene(HSQUIRRELVM v) {
	HSQUIRRELVM vm = g_engine->getVm();
	SQInteger nArgs = sq_gettop(v);

	HSQOBJECT envObj;
	sq_resetobject(&envObj);
	if (SQ_FAILED(sq_getstackobj(v, 1, &envObj)))
		return sq_throwerror(v, "Couldn't get environment from stack");

	// create thread and store it on the stack
	sq_newthread(vm, 1024);
	HSQOBJECT threadObj;
	sq_resetobject(&threadObj);
	if (SQ_FAILED(sq_getstackobj(vm, -1, &threadObj)))
		return sq_throwerror(v, "failed to get coroutine thread from stack");

	// get the closure
	HSQOBJECT closure;
	sq_resetobject(&closure);
	if (SQ_FAILED(sq_getstackobj(v, 2, &closure)))
		return sq_throwerror(v, "failed to get cutscene closure");

	// get the cutscene override closure
	HSQOBJECT closureOverride;
	sq_resetobject(&closureOverride);
	if (nArgs == 3) {
		if (SQ_FAILED(sq_getstackobj(v, 3, &closureOverride)))
			return sq_throwerror(v, "failed to get cutscene override closure");
	}

	Cutscene *cutscene = new Cutscene(v, threadObj, closure, closureOverride, envObj);
	g_engine->_cutscene = cutscene;

	// call the closure in the thread
	cutscene->update(0.f);
	return breakwhilecutscene(v);
}

static SQInteger cutsceneOverride(HSQUIRRELVM v) {
	debug("cutsceneOverride");
	g_engine->_cutscene->cutsceneOverride();
	return 0;
}

static SQInteger dumpvar(HSQUIRRELVM v) {
	warning("TODO: dumpvar: not implemented");
	return 0;
}

static SQInteger exCommand(HSQUIRRELVM v) {
	int cmd;
	if (SQ_FAILED(sqget(v, 2, cmd)))
		return sq_throwerror(v, "Failed to get command");
	switch (cmd) {
	case EX_ALLOW_SAVEGAMES: {
		int enabled;
		if (SQ_FAILED(sqget(v, 3, enabled)))
			return sq_throwerror(v, "Failed to get enabled");
		g_engine->_saveGameManager._allowSaveGame = enabled != 0;
	} break;
	case EX_POP_CHARACTER_SELECTION:
		// seems not to be used
		warning("TODO: exCommand EX_POP_CHARACTER_SELECTION: not implemented");
		break;
	case EX_CAMERA_TRACKING:
		warning("TODO: exCommand EX_CAMERA_TRACKING: not implemented");
		break;
	case EX_RESTART:
		warning("TODO: exCommand EX_RESTART: not implemented");
		break;
	case EX_IDLE_TIME:
		warning("TODO: exCommand EX_IDLE_TIME: not implemented");
		break;
	case EX_DISABLE_SAVESYSTEM:
		warning("TODO: exCommand EX_DISABLE_SAVESYSTEM: not implemented");
		break;
	case EX_OPTIONS_MUSIC:
		warning("TODO: exCommand EX_OPTIONS_MUSIC: not implemented");
		break;
	default:
		warning("exCommand(%dd) not implemented", cmd);
		break;
	}
	return 0;
}

// Returns how long (in seconds) the game has been played for in total (not just this session).
//// Saved when the game is saved.
// Also used for testing.
// The value is a float, so 1 = 1 second, 0.5 = half a second.
//
// . code-block:: Squirrel
// if (gameTime() > (time+testerTronTimeOut)) { // Do something
// }
static SQInteger gameTime(HSQUIRRELVM v) {
	sqpush(v, g_engine->_time);
	return 1;
}

static SQInteger sysInclude(HSQUIRRELVM v) {
	const SQChar *filename;
	if (SQ_FAILED(sqget(v, 2, filename))) {
		return sq_throwerror(v, "failed to get filename");
	}
	g_engine->execNutEntry(v, filename);
	return 0;
}

static SQInteger inputController(HSQUIRRELVM v) {
	warning("TODO: inputController: not implemented");
	return 0;
}

static SQInteger inputHUD(HSQUIRRELVM v) {
	bool on;
	if (SQ_FAILED(sqget(v, 2, on)))
		return sq_throwerror(v, "failed to get on");
	g_engine->_inputState.setInputHUD(on);
	return 0;
}

static SQInteger inputOff(HSQUIRRELVM v) {
	if (!g_engine->_cutscene || g_engine->_cutscene->isStopped()) {
		g_engine->_inputState.setInputActive(false);
		g_engine->_inputState.setShowCursor(false);
	}
	return 0;
}

static SQInteger inputOn(HSQUIRRELVM v) {
	Cutscene *cutscene = g_engine->_cutscene;
	if (!cutscene || cutscene->isStopped()) {
		g_engine->_inputState.setInputActive(true);
		g_engine->_inputState.setShowCursor(true);
	} else {
		int state = g_engine->_inputState.getState();
		state |= UI_INPUT_ON;
		state &= (~UI_INPUT_OFF);
		state |= UI_CURSOR_ON;
		state &= (~UI_CURSOR_OFF);
		cutscene->setInputState((InputStateFlag)state);
		cutscene->setShowCursor(true);
	}
	return 0;
}

static SQInteger inputSilentOff(HSQUIRRELVM v) {
	g_engine->_inputState.setInputActive(false);
	return 0;
}

static SQInteger sysInputState(HSQUIRRELVM v) {
	SQInteger numArgs = sq_gettop(v);
	if (numArgs == 1) {
		int state = (int)g_engine->_inputState.getState();
		sqpush(v, state);
		return 1;
	}
	if (numArgs == 2) {
		int state;
		if (SQ_FAILED(sqget(v, 2, state)))
			return sq_throwerror(v, "failed to get state");
		g_engine->_inputState.setState((InputStateFlag)state);
		return 0;
	}
	return sq_throwerror(v, Common::String::format("inputState with %lld arguments not implemented", numArgs).c_str());
}

static SQInteger inputVerbs(HSQUIRRELVM v) {
	bool on;
	if (SQ_FAILED(sqget(v, 2, on)))
		return sq_throwerror(v, "failed to get isActive");
	debug("inputVerbs: %s", on ? "yes" : "no");
	g_engine->_inputState.setInputVerbsActive(on);
	return 1;
}

static SQInteger isInputOn(HSQUIRRELVM v) {
	bool isActive = g_engine->_inputState.getInputActive();
	sqpush(v, isActive);
	return 1;
}

static SQInteger logEvent(HSQUIRRELVM v) {
	SQInteger numArgs = sq_gettop(v);
	Common::String msg, event;
	if (SQ_FAILED(sqget(v, 2, event)))
		return sq_throwerror(v, "failed to get event");
	if (numArgs == 3) {
		if (SQ_FAILED(sqget(v, 3, event)))
			return sq_throwerror(v, "failed to get message");
		msg = event + ": " + msg;
	}
	debug("%s", msg.c_str());
	return 0;
}

// Like a print statement, but gets sent to the output log file instead.
// Useful for testing.
static SQInteger logInfo(HSQUIRRELVM v) {
	Common::String msg;
	if (SQ_FAILED(sqget(v, 2, msg)))
		return sq_throwerror(v, "failed to get message");
	debug("%s", msg.c_str());
	return 0;
}

// Sends a warning message to the output log file.
static SQInteger logWarning(HSQUIRRELVM v) {
	Common::String msg;
	if (SQ_FAILED(sqget(v, 2, msg)))
		return sq_throwerror(v, "failed to get message");
	warning("%s", msg.c_str());
	return 0;
}

// Returns game time in milliseconds.
// Based on when the machine is booted and runs all the time (not paused or saved).
// See also gameTime, which is in seconds.
static SQInteger microTime(HSQUIRRELVM v) {
	sqpush(v, g_engine->_time * 1000.0f);
	return 1;
}

static SQInteger moveCursorTo(HSQUIRRELVM v) {
	int x, y;
	if (SQ_FAILED(sqget(v, 2, x)))
		return sq_throwerror(v, "Failed to get x");
	if (SQ_FAILED(sqget(v, 3, y)))
		return sq_throwerror(v, "Failed to get y");
	float t;
	if (SQ_FAILED(sqget(v, 4, t)))
		return sq_throwerror(v, "Failed to get time");

	g_engine->_cursor.pos = Math::Vector2d(x, y);
	// TODO: use time
	return 0;
}

// removeCallback(id: int) remove the given callback
static SQInteger removeCallback(HSQUIRRELVM v) {
	int id = 0;
	if (SQ_FAILED(sqget(v, 2, id)))
		return sq_throwerror(v, "failed to get callback");
	for (size_t i = 0; i < g_engine->_callbacks.size(); i++) {
		Callback *cb = g_engine->_callbacks[i];
		if (cb->getId() == id) {
			g_engine->_callbacks.remove_at(i);
			delete cb;
			return 0;
		}
	}
	return 0;
}

static SQInteger setAmbientLight(HSQUIRRELVM v) {
	warning("TODO: setAmbientLight: not implemented");
	return 0;
}

static SQInteger startthread(HSQUIRRELVM v) {
	return _startthread(v, false);
}

static SQInteger startglobalthread(HSQUIRRELVM v) {
	return _startthread(v, true);
}

// Stops a thread specified by threadid.
//
// If the thread is not running, the command does nothing.
//
// See also:
// * `startthread`
// * `startglobalthread`
static SQInteger stopthread(HSQUIRRELVM v) {
	int id = 0;
	if (SQ_FAILED(sqget(v, 2, id))) {
		sqpush(v, 0);
		return 1;
	}

	ThreadBase *t = sqthread(id);
	if (t) {
		t->stop();
	}

	sqpush(v, 0);
	return 1;
}

// Returns the thread ID of the currently running script/thread.
//
// If no thread is running, it will return 0.
//
// . code-block:: Squirrel
// Phone <-
// {
//     phoneRingingTID = 0
//     script phoneRinging(number) {
//         phoneRingingTID = threadid()
//         ...
//     }
//     function clickedButton(...) {
//         if (!phoneRingingTID) {
//             ...
//         }
//     }
// }
static SQInteger threadid(HSQUIRRELVM v) {
	ThreadBase *t = sqthread(v);
	if (t)
		sqpush(v, t->getId());
	else
		sqpush(v, 0);
	return 1;
}

// Specify whether a thread should be pauseable or not.
// If a thread is not pauseable, it won't be possible to pause this thread.
static SQInteger threadpauseable(HSQUIRRELVM v) {
	ThreadBase *t = sqthread(v, 2);
	if (!t)
		return sq_throwerror(v, "failed to get thread");
	int pauseable = 0;
	if (SQ_FAILED(sqget(v, 3, pauseable)))
		return sq_throwerror(v, "failed to get pauseable");
	t->_pauseable = pauseable != 0;
	return 0;
}

void sqgame_register_syslib(HSQUIRRELVM v) {
	regFunc(v, startthread, _SC("startthread"));
	regFunc(v, startglobalthread, _SC("startglobalthread"));
	regFunc(v, breaktime, _SC("breaktime"));
	regFunc(v, activeController, _SC("activeController"));
	regFunc(v, addCallback, _SC("addCallback"));
	regFunc(v, addFolder, _SC("addFolder"));
	regFunc(v, breakhere, _SC("breakhere"));
	regFunc(v, breaktime, _SC("breaktime"));
	regFunc(v, breakwhileanimating, _SC("breakwhileanimating"));
	regFunc(v, breakwhilecamera, _SC("breakwhilecamera"));
	regFunc(v, breakwhilecutscene, _SC("breakwhilecutscene"));
	regFunc(v, breakwhiledialog, _SC("breakwhiledialog"));
	regFunc(v, breakwhileinputoff, _SC("breakwhileinputoff"));
	regFunc(v, breakwhilerunning, _SC("breakwhilerunning"));
	regFunc(v, breakwhilesound, _SC("breakwhilesound"));
	regFunc(v, breakwhiletalking, _SC("breakwhiletalking"));
	regFunc(v, breakwhilewalking, _SC("breakwhilewalking"));
	regFunc(v, cutscene, _SC("cutscene"));
	regFunc(v, cutsceneOverride, _SC("cutsceneOverride"));
	regFunc(v, dumpvar, _SC("dumpvar"));
	regFunc(v, exCommand, _SC("exCommand"));
	regFunc(v, gameTime, _SC("gameTime"));
	regFunc(v, sysInclude, _SC("include"));
	regFunc(v, inputController, _SC("inputController"));
	regFunc(v, inputHUD, _SC("inputHUD"));
	regFunc(v, inputOff, _SC("inputOff"));
	regFunc(v, inputOn, _SC("inputOn"));
	regFunc(v, inputSilentOff, _SC("inputSilentOff"));
	regFunc(v, sysInputState, _SC("inputState"));
	regFunc(v, inputVerbs, _SC("inputVerbs"));
	regFunc(v, isInputOn, _SC("isInputOn"));
	regFunc(v, logEvent, _SC("logEvent"));
	regFunc(v, logInfo, _SC("logInfo"));
	regFunc(v, logWarning, _SC("logWarning"));
	regFunc(v, microTime, _SC("microTime"));
	regFunc(v, moveCursorTo, _SC("moveCursorTo"));
	regFunc(v, removeCallback, _SC("removeCallback"));
	regFunc(v, setAmbientLight, _SC("setAmbientLight"));
	regFunc(v, startglobalthread, _SC("startglobalthread"));
	regFunc(v, startthread, _SC("startthread"));
	regFunc(v, stopthread, _SC("stopthread"));
	regFunc(v, threadid, _SC("threadid"));
	regFunc(v, threadpauseable, _SC("threadpauseable"));
}

static void regConst(HSQUIRRELVM v, const char *name, int value) {
	SQObject obj = sqtoobj(v, value);
	_table(v->_roottable)->NewSlot(sqtoobj(v, name), SQObjectPtr(obj));
}

void sqgame_register_constants(HSQUIRRELVM v) {
	regConst(v, "ALL", ALL);
	regConst(v, "HERE", HERE);
	regConst(v, "GONE", GONE);
	regConst(v, "OFF", OFF);
	regConst(v, "ON", ON);
	regConst(v, "FULL", FULL);
	regConst(v, "EMPTY", EMPTY);
	regConst(v, "OPEN", OPEN);
	regConst(v, "CLOSED", CLOSED);
	regConst(v, "FALSE", FALSE);
	regConst(v, "TRUE", TRUE);
	regConst(v, "MOUSE", MOUSE);
	regConst(v, "CONTROLLER", CONTROLLER);
	regConst(v, "DIRECTDRIVE", DIRECTDRIVE);
	regConst(v, "TOUCH", TOUCH);
	regConst(v, "REMOTE", REMOTE);
	regConst(v, "FADE_IN", FADE_IN);
	regConst(v, "FADE_OUT", FADE_OUT);
	regConst(v, "FADE_WOBBLE", FADE_WOBBLE);
	regConst(v, "FADE_WOBBLE_TO_SEPIA", FADE_WOBBLE_TO_SEPIA);
	regConst(v, "FACE_FRONT", FACE_FRONT);
	regConst(v, "FACE_BACK", FACE_BACK);
	regConst(v, "FACE_LEFT", FACE_LEFT);
	regConst(v, "FACE_RIGHT", FACE_RIGHT);
	regConst(v, "FACE_FLIP", FACE_FLIP);
	regConst(v, "DIR_FRONT", DIR_FRONT);
	regConst(v, "DIR_BACK", DIR_BACK);
	regConst(v, "DIR_LEFT", DIR_LEFT);
	regConst(v, "DIR_RIGHT", DIR_RIGHT);
	regConst(v, "LINEAR", LINEAR);
	regConst(v, "EASE_IN", EASE_IN);
	regConst(v, "EASE_INOUT", EASE_INOUT);
	regConst(v, "EASE_OUT", EASE_OUT);
	regConst(v, "SLOW_EASE_IN", SLOW_EASE_IN);
	regConst(v, "SLOW_EASE_OUT", SLOW_EASE_OUT);
	regConst(v, "LOOPING", LOOPING);
	regConst(v, "SWING", SWING);
	regConst(v, "ALIGN_LEFT", ALIGN_LEFT);
	regConst(v, "ALIGN_CENTER", ALIGN_CENTER);
	regConst(v, "ALIGN_RIGHT", ALIGN_RIGHT);
	regConst(v, "ALIGN_TOP", ALIGN_TOP);
	regConst(v, "ALIGN_BOTTOM", ALIGN_BOTTOM);
	regConst(v, "LESS_SPACING", LESS_SPACING);
	regConst(v, "EX_ALLOW_SAVEGAMES", EX_ALLOW_SAVEGAMES);
	regConst(v, "EX_POP_CHARACTER_SELECTION", EX_POP_CHARACTER_SELECTION);
	regConst(v, "EX_CAMERA_TRACKING", EX_CAMERA_TRACKING);
	regConst(v, "EX_BUTTON_HOVER_SOUND", EX_BUTTON_HOVER_SOUND);
	regConst(v, "EX_RESTART", EX_RESTART);
	regConst(v, "EX_IDLE_TIME", EX_IDLE_TIME);
	regConst(v, "EX_AUTOSAVE", EX_AUTOSAVE);
	regConst(v, "EX_AUTOSAVE_STATE", EX_AUTOSAVE_STATE);
	regConst(v, "EX_DISABLE_SAVESYSTEM", EX_DISABLE_SAVESYSTEM);
	regConst(v, "EX_SHOW_OPTIONS", EX_SHOW_OPTIONS);
	regConst(v, "EX_OPTIONS_MUSIC", EX_OPTIONS_MUSIC);
	regConst(v, "EX_FORCE_TALKIE_TEXT", EX_FORCE_TALKIE_TEXT);
	regConst(v, "GRASS_BACKANDFORTH", GRASS_BACKANDFORTH);
	regConst(v, "EFFECT_NONE", EFFECT_NONE);
	regConst(v, "DOOR", DOOR);
	regConst(v, "DOOR_LEFT", DOOR_LEFT);
	regConst(v, "DOOR_RIGHT", DOOR_RIGHT);
	regConst(v, "DOOR_BACK", DOOR_BACK);
	regConst(v, "DOOR_FRONT", DOOR_FRONT);
	regConst(v, "FAR_LOOK", FAR_LOOK);
	regConst(v, "USE_WITH", USE_WITH);
	regConst(v, "USE_ON", USE_ON);
	regConst(v, "USE_IN", USE_IN);
	regConst(v, "GIVEABLE", GIVEABLE);
	regConst(v, "TALKABLE", TALKABLE);
	regConst(v, "IMMEDIATE", IMMEDIATE);
	regConst(v, "FEMALE", FEMALE);
	regConst(v, "MALE", MALE);
	regConst(v, "PERSON", PERSON);
	regConst(v, "REACH_HIGH", REACH_HIGH);
	regConst(v, "REACH_MED", REACH_MED);
	regConst(v, "REACH_LOW", REACH_LOW);
	regConst(v, "REACH_NONE", REACH_NONE);
	regConst(v, "VERB_WALKTO", VERB_WALKTO);
	regConst(v, "VERB_LOOKAT", VERB_LOOKAT);
	regConst(v, "VERB_TALKTO", VERB_TALKTO);
	regConst(v, "VERB_PICKUP", VERB_PICKUP);
	regConst(v, "VERB_OPEN", VERB_OPEN);
	regConst(v, "VERB_CLOSE", VERB_CLOSE);
	regConst(v, "VERB_PUSH", VERB_PUSH);
	regConst(v, "VERB_PULL", VERB_PULL);
	regConst(v, "VERB_GIVE", VERB_GIVE);
	regConst(v, "VERB_USE", VERB_USE);
	regConst(v, "VERB_DIALOG", VERB_DIALOG);
	regConst(v, "VERBFLAG_INSTANT", VERBFLAG_INSTANT);
	regConst(v, "NO", TWP_NO);
	regConst(v, "YES", TWP_YES);
	regConst(v, "UNSELECTABLE", UNSELECTABLE);
	regConst(v, "SELECTABLE", SELECTABLE);
	regConst(v, "TEMP_UNSELECTABLE", TEMP_UNSELECTABLE);
	regConst(v, "TEMP_SELECTABLE", TEMP_SELECTABLE);
	regConst(v, "MAC", MAC);
	regConst(v, "WIN", WIN);
	regConst(v, "LINUX", LINUX);
	regConst(v, "XBOX", XBOX);
	regConst(v, "IOS", IOS);
	regConst(v, "ANDROID", ANDROID);
	regConst(v, "SWITCH", SWITCH);
	regConst(v, "PS4", PS4);
	regConst(v, "EFFECT_NONE", EFFECT_NONE);
	regConst(v, "EFFECT_SEPIA", EFFECT_SEPIA);
	regConst(v, "EFFECT_EGA", EFFECT_EGA);
	regConst(v, "EFFECT_VHS", EFFECT_VHS);
	regConst(v, "EFFECT_GHOST", EFFECT_GHOST);
	regConst(v, "EFFECT_BLACKANDWHITE", EFFECT_BLACKANDWHITE);
	regConst(v, "UI_INPUT_ON", UI_INPUT_ON);
	regConst(v, "UI_INPUT_OFF", UI_INPUT_OFF);
	regConst(v, "UI_VERBS_ON", UI_VERBS_ON);
	regConst(v, "UI_VERBS_OFF", UI_VERBS_OFF);
	regConst(v, "UI_HUDOBJECTS_ON", UI_HUDOBJECTS_ON);
	regConst(v, "UI_HUDOBJECTS_OFF", UI_HUDOBJECTS_OFF);
	regConst(v, "UI_CURSOR_ON", UI_CURSOR_ON);
	regConst(v, "UI_CURSOR_OFF", UI_CURSOR_OFF);
	regConst(v, "KEY_UP", KEY_UP);
	regConst(v, "KEY_RIGHT", KEY_RIGHT);
	regConst(v, "KEY_DOWN", KEY_DOWN);
	regConst(v, "KEY_LEFT", KEY_LEFT);
	regConst(v, "KEY_PAD1", KEY_PAD1);
	regConst(v, "KEY_PAD2", KEY_PAD2);
	regConst(v, "KEY_PAD3", KEY_PAD3);
	regConst(v, "KEY_PAD4", KEY_PAD4);
	regConst(v, "KEY_PAD5", KEY_PAD5);
	regConst(v, "KEY_PAD6", KEY_PAD6);
	regConst(v, "KEY_PAD7", KEY_PAD7);
	regConst(v, "KEY_PAD8", KEY_PAD8);
	regConst(v, "KEY_PAD9", KEY_PAD9);
	regConst(v, "KEY_ESCAPE", KEY_ESCAPE);
	regConst(v, "KEY_TAB", KEY_TAB);
	regConst(v, "KEY_RETURN", KEY_RETURN);
	regConst(v, "KEY_BACKSPACE", KEY_BACKSPACE);
	regConst(v, "KEY_SPACE", KEY_SPACE);
	regConst(v, "KEY_A", KEY_A);
	regConst(v, "KEY_B", KEY_B);
	regConst(v, "KEY_C", KEY_C);
	regConst(v, "KEY_D", KEY_D);
	regConst(v, "KEY_E", KEY_E);
	regConst(v, "KEY_F", KEY_F);
	regConst(v, "KEY_G", KEY_G);
	regConst(v, "KEY_H", KEY_H);
	regConst(v, "KEY_I", KEY_I);
	regConst(v, "KEY_J", KEY_J);
	regConst(v, "KEY_K", KEY_K);
	regConst(v, "KEY_L", KEY_L);
	regConst(v, "KEY_M", KEY_M);
	regConst(v, "KEY_N", KEY_N);
	regConst(v, "KEY_O", KEY_O);
	regConst(v, "KEY_P", KEY_P);
	regConst(v, "KEY_Q", KEY_Q);
	regConst(v, "KEY_R", KEY_R);
	regConst(v, "KEY_S", KEY_S);
	regConst(v, "KEY_T", KEY_T);
	regConst(v, "KEY_U", KEY_U);
	regConst(v, "KEY_V", KEY_V);
	regConst(v, "KEY_W", KEY_W);
	regConst(v, "KEY_X", KEY_X);
	regConst(v, "KEY_Y", KEY_Y);
	regConst(v, "KEY_Z", KEY_Z);
	regConst(v, "KEY_0", KEY_0);
	regConst(v, "KEY_1", KEY_1);
	regConst(v, "KEY_2", KEY_2);
	regConst(v, "KEY_3", KEY_3);
	regConst(v, "KEY_4", KEY_4);
	regConst(v, "KEY_5", KEY_5);
	regConst(v, "KEY_6", KEY_6);
	regConst(v, "KEY_7", KEY_7);
	regConst(v, "KEY_8", KEY_8);
	regConst(v, "KEY_9", KEY_9);
	regConst(v, "KEY_F1", KEY_F1);
	regConst(v, "KEY_F2", KEY_F2);
	regConst(v, "KEY_F3", KEY_F3);
	regConst(v, "KEY_F4", KEY_F4);
	regConst(v, "KEY_F5", KEY_F5);
	regConst(v, "KEY_F6", KEY_F6);
	regConst(v, "KEY_F7", KEY_F7);
	regConst(v, "KEY_F8", KEY_F8);
	regConst(v, "KEY_F9", KEY_F9);
	regConst(v, "KEY_F10", KEY_F10);
	regConst(v, "KEY_F11", KEY_F11);
	regConst(v, "KEY_F12", KEY_F12);
	regConst(v, "BUTTON_A", BUTTON_A);
	regConst(v, "BUTTON_B", BUTTON_B);
	regConst(v, "BUTTON_X", BUTTON_X);
	regConst(v, "BUTTON_Y", BUTTON_Y);
	regConst(v, "BUTTON_START", BUTTON_START);
	regConst(v, "BUTTON_BACK", BUTTON_BACK);
	regConst(v, "BUTTON_MOUSE_LEFT", BUTTON_MOUSE_LEFT);
	regConst(v, "BUTTON_MOUSE_RIGHT", BUTTON_MOUSE_RIGHT);
	regConst(v, "WAITING_FOR_CHOICE", 2);
	regConst(v, "PLATFORM", 1); // TODO: choose the right platform
}

} // namespace Twp
