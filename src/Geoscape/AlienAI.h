/*
 * Copyright 2010-2012 OpenXcom Developers.
 *
 * This file is part of OpenXcom.
 *
 * OpenXcom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenXcom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenXcom.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef OPENXCOM_ALIENAI_H
#define OPENXCOM_ALIENAI_H

#include "GeoEventHandler.h"

namespace OpenXcom
{

class Game;
class GeoscapeState;

/**
 * This class handles the Alien side's actions.
 * It reacts to events happening in the Geoscape.
 */
class AlienAI: public GeoEventHandler
{
public:
	/// Create a new AlienAI.
	AlienAI(Game *game, GeoscapeState *parent);

protected:
	void handle(const Every30Minutes &);
	void handle(const EveryMonth &);
	/// Access to game data and rules.
	Game &_game;
	/// Access to parent state.
	GeoscapeState &_parent;
};

}

#endif
