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
 * aint32 with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *
 * Based on the original sources
 *   Faery Tale II -- The Halls of the Dead
 *   (c) 1993-1996 The Wyrmkeep Entertainment Co.
 */

#include "saga2/std.h"
#include "saga2/fta.h"
#include "saga2/patrol.h"
#include "saga2/cmisc.h"
#include "saga2/tcoords.h"
#include "saga2/hresmgr.h"

namespace Saga2 {

typedef PatrolRouteData *PatrolRouteDataPtr;

extern int16 worldCount;						// Number of worlds

PatrolRouteList *patrolRouteList = nullptr;		// Global patrol route array

static PatrolRouteData **patrolRouteData;		// Data for patrol routes


// Returns a const reference to a specified way point
const TilePoint &PatrolRoute::operator [](int16 index) const {
	return *((TilePoint *)&this[1] + index);
}


/* ===================================================================== *
   PatrolRouteList member functions
 * ===================================================================== */
void PatrolRouteList::clearRouteData(void) {
	if (_offsetArray) {
		delete[] _offsetArray;
		_offsetArray = nullptr;
	}

	_routeData = nullptr;
}

//-----------------------------------------------------------------------
//	Set up and initialize new patrol route data
void PatrolRouteList::setRouteData(PatrolRouteData *data) {
	int16 i, noRoutes = data->routes;
	PatrolRoute *currentRoute;

	// If routeData is nullptr simply return
	if ((_routeData = data) == nullptr) {
		_offsetArray = nullptr;
		return;
	}

	// Allocate a new offsetArray
	_offsetArray = new int32[noRoutes]();

	if (_offsetArray == nullptr)
		error("Cannot allocate patrol route list offset array.");

	// Iterate through each patrol route a compute its offset
	for (i = 0, currentRoute = (PatrolRoute *)&_routeData[1]; i < noRoutes; i++) {
		warning("STUB: PatrolRouteList::setRouteData: unsafe arithmetics");
		_offsetArray[i] = 0; // FIXME: It was "currentRoute - routeData";

		currentRoute = (PatrolRoute *)&(*currentRoute)[currentRoute->vertices()];
	}
}

/* ===================================================================== *
   PatrolRouteIterator member functions
 * ===================================================================== */

PatrolRouteIterator::PatrolRouteIterator(uint8 map, int16 rte, uint8 type) :
		_mapNum(map), _routeNo(rte), _flags(type & 0xF) {
	const PatrolRoute &route = patrolRouteList[_mapNum][_routeNo];

	if (_flags & patrolRouteRandom)
		_vertexNo = g_vm->_rnd->getRandomNumber(route.vertices() - 1);
	else {
		if (_flags & patrolRouteReverse)
			_vertexNo = route.vertices() - 1;
		else
			_vertexNo = 0;
	}
}

PatrolRouteIterator::PatrolRouteIterator(uint8 map, int16 rte, uint8 type, int16 startingPoint) :
		_mapNum(map), _routeNo(rte), _flags(type & 0xF) {
	const PatrolRoute &route = patrolRouteList[_mapNum][_routeNo];

	_vertexNo = clamp(0, startingPoint, route.vertices() - 1);
}

//-----------------------------------------------------------------------
//	Increment the waypoint index
void PatrolRouteIterator::increment(void) {
	const PatrolRoute &route = patrolRouteList[_mapNum][_routeNo];

	_vertexNo++;

	if (_vertexNo >= route.vertices()) {
		if (_flags & patrolRouteAlternate) {
			// If alternating, initialize for iteration in the alternate
			// direction
			_flags |= patrolRouteInAlternate;
			_vertexNo = MAX(route.vertices() - 2, 0);
		} else if (_flags & patrolRouteRepeat)
			// If repeating, reset the waypoint index
			_vertexNo = 0;
	}
}

//-----------------------------------------------------------------------
//	Decrement the waypoint index
void PatrolRouteIterator::decrement(void) {
	const PatrolRoute &route = patrolRouteList[_mapNum][_routeNo];

	_vertexNo--;

	if (_vertexNo < 0) {
		if (_flags & patrolRouteAlternate) {
			// If alternating, initialize for iteration in the alternate
			// direction
			_flags |= patrolRouteInAlternate;
			_vertexNo = MIN(1, route.vertices() - 1);
		} else if (_flags & patrolRouteRepeat)
			// If repeating, reset the waypoint index
			_vertexNo = route.vertices() - 1;
	}
}

//-----------------------------------------------------------------------
//	Increment the waypoint index in the alternate direction
void PatrolRouteIterator::altIncrement(void) {
	const PatrolRoute   &route = patrolRouteList[_mapNum][_routeNo];

	_vertexNo++;

	if (_vertexNo >= route.vertices() && (_flags & patrolRouteRepeat)) {
		// If repeating, initialize for iteration in the standard
		// direction, and reset the waypoint index
		_flags &= ~patrolRouteInAlternate;
		_vertexNo = MAX(route.vertices() - 2, 0);
	}
}

//-----------------------------------------------------------------------
//	Decrement the waypoint index in the alternate direction
void PatrolRouteIterator::altDecrement(void) {
	const PatrolRoute &route = patrolRouteList[_mapNum][_routeNo];

	_vertexNo--;

	if (_vertexNo < 0 && (_flags & patrolRouteRepeat)) {
		// If repeating, initialize for iteration in the standard
		// direction, and reset the waypoint index
		_flags &= ~patrolRouteInAlternate;
		_vertexNo = MIN(1, route.vertices() - 1);
	}
}

//-----------------------------------------------------------------------
//	Return the coordinates of the current waypoint
const TilePoint &PatrolRouteIterator::operator*(void) const {
	const PatrolRoute &route = patrolRouteList[_mapNum][_routeNo];

	return _vertexNo >= 0 && _vertexNo < route.vertices() ? route[_vertexNo] : Nowhere;
}

const PatrolRouteIterator &PatrolRouteIterator::operator++(void) {
	const PatrolRoute &route = patrolRouteList[_mapNum][_routeNo];

	if (_vertexNo >= 0 & _vertexNo < route.vertices()) {
		if (!(_flags & patrolRouteRandom)) {
			if (!(_flags & patrolRouteInAlternate)) {
				if (!(_flags & patrolRouteReverse))
					increment();
				else
					decrement();
			} else {
				if (!(_flags & patrolRouteReverse))
					altDecrement();
				else
					altIncrement();
			}
		} else {
			_vertexNo = g_vm->_rnd->getRandomNumber(route.vertices() - 1);
		}
	}

	return *this;
}

//-----------------------------------------------------------------------
//	Load the patrol routes from the resource file
void initPatrolRoutes(void) {
	int16 i;
	hResContext *patrolRouteRes;

	// Get patrol route resource context
	patrolRouteRes = auxResFile->newContext(MKTAG('P', 'T', 'R', 'L'), "patrol route resource");
	if (patrolRouteRes == nullptr || !patrolRouteRes->_valid)
		error("Error accessing patrol route resource group.");

	// Allocate the patrol route list array
	patrolRouteList = new PatrolRouteList[worldCount];

	if (patrolRouteList == nullptr)
		error("Unable to allocate the patrol route list");

	// Allocate the patrol route data pointer array
	patrolRouteData = new PatrolRouteDataPtr[worldCount];

	if (patrolRouteData == nullptr)
		error("Unable to allocate the patrol route data pointers");

	for (i = 0; i < worldCount; i++) {
		// Initialize the patrol route data members
		patrolRouteList[i]._routeData = nullptr;
		patrolRouteList[i]._offsetArray = nullptr;

		// Load this worlds's patrol routes
		if (patrolRouteRes->size(MKTAG('R', 'T', 'E', i)) > 0) {
			patrolRouteData[i] = (PatrolRouteData *)LoadResource(patrolRouteRes, MKTAG('R', 'T', 'E', i), "patrol route data");

			if (patrolRouteData[i] == nullptr)
				error("Unable to load the patrol route data");

			// Initialize the PatrolRouteList with the resource data
			patrolRouteList[i].setRouteData(patrolRouteData[i]);
		} else
			patrolRouteData[i] = nullptr;
	}

	// Dispose of the patrol route resource context
	auxResFile->disposeContext(patrolRouteRes);
}

void cleanupPatrolRoutes(void) {
	int16 i;

	for (i = 0; i < worldCount; i++) {
		patrolRouteList[i].clearRouteData();
		if (patrolRouteData[i])
			free(patrolRouteData[i]);
	}

	delete[] patrolRouteData;
	delete[] patrolRouteList;
}

} // end of namespace Saga2
