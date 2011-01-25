/*
 * Copyright 2010 OpenXcom Developers.
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
#define _USE_MATH_DEFINES
#include <cmath>
#include "TerrainModifier.h"
#include "SDL.h"
#include "../Savegame/SavedBattleGame.h"
#include "../Savegame/Tile.h"
#include "../Savegame/BattleUnit.h"
#include "../Engine/RNG.h"

namespace OpenXcom
{

/**
 * Sets up a TerrainModifier.
 * @param save pointer to SavedBattleGame object.
 */
TerrainModifier::TerrainModifier(SavedBattleGame *save) : _save(save)
{

}

/**
 * Deletes the TerrainModifier.
 */
TerrainModifier::~TerrainModifier()
{

}

/**
  * Calculate sun shading for the whole terrain.
  */
void TerrainModifier::calculateSunShading()
{
	for (int i = 0; i < _save->getWidth() * _save->getLength() * _save->getHeight(); i++)
	{
		calculateSunShading(_save->getTiles()[i]);
	}	
}

/**
  * Calculate sun shading for 1 tile. Sun comes from above and is blocked by floors or objects.
  * @param tile The tile to calculate sun shading for.
  */
void TerrainModifier::calculateSunShading(Tile *tile)
{

	// At night don't check for sun shading.
	if (_save->getGlobalShade() > 5)
	{
		tile->setShadeSun(_save->getGlobalShade());
		return;
	}

	if (tile->getPosition().z < _save->getHeight() - 1
		&& ((_save->getTile(tile->getPosition() + Position(0,0,1))
		&& (_save->getTile(tile->getPosition() + Position(0,0,1))->getMapData(O_FLOOR)
		|| _save->getTile(tile->getPosition() + Position(0,0,1))->getMapData(O_OBJECT)))
			|| (_save->getTile(tile->getPosition() + Position(0,0,2))
		&& (_save->getTile(tile->getPosition() + Position(0,0,2))->getMapData(O_FLOOR)
		|| _save->getTile(tile->getPosition() + Position(0,0,2))->getMapData(O_OBJECT))))
		)
	{
		tile->setShadeSun(_save->getGlobalShade() + (4 - (_save->getGlobalShade()/2)));
	}
	else
	{
		tile->setShadeSun(_save->getGlobalShade());
	}
}

/**
  * Recalculate lighting for the complete battlescape: units,terrain,items,fire.
  */
void TerrainModifier::calculateLighting()
{
	// reset all light to 0 first
	for (int i = 0; i < _save->getWidth() * _save->getLength() * _save->getHeight(); i++)
	{
		_save->getTiles()[i]->resetLight();
	}

	// add lighting of terrain
	for (int i = 0; i < _save->getWidth() * _save->getLength() * _save->getHeight(); i++)
	{
		// only floors and objects can light up
		if (_save->getTiles()[i]->getMapData(O_FLOOR)
			&& _save->getTiles()[i]->getMapData(O_FLOOR)->getLightSource())
		{
			circularRaytracing(_save->getTiles()[i]->getPosition(), AFFECT_LIGHT, _save->getTiles()[i]->getMapData(O_FLOOR)->getLightSource());
		}
		if (_save->getTiles()[i]->getMapData(O_OBJECT)
			&& _save->getTiles()[i]->getMapData(O_OBJECT)->getLightSource())
		{
			circularRaytracing(_save->getTiles()[i]->getPosition(), AFFECT_LIGHT, _save->getTiles()[i]->getMapData(O_OBJECT)->getLightSource());
		}

		// fire
		if (_save->getTiles()[i]->getFire() > 0)
		{
			circularRaytracing(_save->getTiles()[i]->getPosition(), AFFECT_LIGHT, 15);
		}

	}

	// add lighting of items (flares)

	// add lighting of soldiers + recalculate line of sight
	for (std::vector<BattleUnit*>::iterator i = _save->getUnits()->begin(); i != _save->getUnits()->end(); i++)
	{
		if ((*i)->getFaction() == FACTION_PLAYER)
		{
			circularRaytracing((*i)->getPosition(), AFFECT_LIGHT, 15);
			calculateLineOfSight(*i);
		}
	}

	// indicate we have finished recalculating
	for (int i = 0; i < _save->getWidth() * _save->getLength() * _save->getHeight(); i++)
	{
		_save->getTiles()[i]->setLight();
	}
}

/**
 * Destroys all parts of a tile. This is used for testing purposes (press 'd' on battlescape)
 * @param tile
 */
void TerrainModifier::destroyTile(Tile *tile)
{
	tile->destroy(0);
	tile->destroy(1);
	tile->destroy(2);
	tile->destroy(3);
	if (tile->getPosition().z > 0)
	{
		tile->setMapData(0, O_FLOOR);
	}
	for (int i=0; i < _save->getHeight() - 1; i++)
	{
		calculateSunShading(_save->getTile(Position(tile->getPosition().x, tile->getPosition().y, i)));
	}
	calculateLighting();
}

/**
 * Calculates line of sight of a soldier. Every visible tile is marked + fog of war is removed.
 * @param unit
 */
void TerrainModifier::calculateLineOfSight(BattleUnit *unit)
{
	double startAngle[8] = { 45, 0, -45, 270, 225, 180, 135, 90 };
	double endAngle[8] = { 135, 90, 45, 360, 315, 270, 225, 180 };
	circularRaytracing(unit->getPosition(), AFFECT_VISION, 20, startAngle[unit->getDirection()], endAngle[unit->getDirection()], unit);
}

/**
 * Traces tiles following rays in a circular pattern. Can be used for light and all kinds of explosions (HE,smoke,fire,stun).
 * @param center The position where the circular pattern starts.
 * @param affector The kind of affector that affects the tiles.
 * @param power The initial power at the point of impact.
 * @param startAngle The angle at wich the circle starts.
 * @param stopAngle The angle at wich the circle stops.
 * @param unit The unit causing this effect.
 */
void TerrainModifier::circularRaytracing(const Position &center, Affector affector, int power, double startAngle, double stopAngle, BattleUnit *unit)
{
	double centerX = center.x + 0.5;
	double centerY = center.y + 0.5;
	int power_, startPower;

	// a unique ID for this session, used to avoid tiles to be affected more than once.
	int sessionID = RNG::generate(1,65000);
	
	// commenting the for loop will shine light only on current layer
	for (int centerZ = 0; centerZ < _save->getHeight(); centerZ++)
	{
		startPower = power;

		// We can not see on this level, because a floor above it is blocking it. Continue to next level.
		if (centerZ < center.z && affector != AFFECT_LIGHT && !_save->getTile(Position(center.x,center.y,centerZ + 1))->hasNoFloor())
			continue;
		// We can not see on this level, because the tile above us has a floor. No need to check further - break here.
		if (centerZ > center.z && affector != AFFECT_LIGHT && !_save->getTile(Position(center.x,center.y,centerZ))->hasNoFloor())
			break;

		startPower -= abs(centerZ - center.z) * 2;

		// the center is affected for sure
		if (affector == AFFECT_LIGHT)
			_save->getTile(Position(center.x,center.y,centerZ))->addLight(startPower, sessionID);
		if (affector == AFFECT_VISION) 
			_save->getTile(Position(center.x,center.y,centerZ))->isSeenBy(unit, sessionID);

		// raytrace every 3 degrees makes sure we cover all tiles in a circle.
		for (double te = startAngle; te <= stopAngle; te += 3)
		{
			double cos_te = cos(te * M_PI / 180.0);
			double sin_te = sin(te * M_PI / 180.0);

			double oz = centerZ, ox = centerX, oy = centerY;
			int l = 0;
			double vz, vx, vy;
			power_ = startPower;
			while (power_ > 0)
			{
				l++;
				vz = centerZ;
				vx = centerX + l * cos_te;
				vy = centerY + l * sin_te;

				Tile *t = _save->getTile(Position(int(floor(vx)),int(floor(vy)),int(floor(vz))));
				if (!t) break;
				if (affector != AFFECT_LIGHT)
					power_ -= blockage(_save->getTile(Position(int(floor(ox)),int(floor(oy)),int(floor(oz)))), t, affector);
				power_--;
				if (power_ > 0)
				{
					if (affector == AFFECT_LIGHT)
						t->addLight(power_, sessionID);
					if (affector == AFFECT_VISION) 
					{
						t->isSeenBy(unit, sessionID);
						// if there is a door to the east or south of a visible tile, we see that too
						t = _save->getTile(Position(int(floor(vx) + 1), int(floor(vy)), int(floor(vz))));
						if (t && t->getMapData(O_WESTWALL) && (t->getMapData(O_WESTWALL)->isDoor() || t->getMapData(O_WESTWALL)->isUFODoor()))
						{
							t->isSeenBy(unit, sessionID);
						}
						t = _save->getTile(Position(int(floor(vx)), int(floor(vy) - 1), int(floor(vz))));
						if (t && t->getMapData(O_NORTHWALL) && (t->getMapData(O_NORTHWALL)->isDoor() || t->getMapData(O_NORTHWALL)->isUFODoor()))
						{
							t->isSeenBy(unit, sessionID);
						}
					}

				}
				oz=vz;ox=vx;oy=vy;
			}
		}
	}
}
/*
 * The amount this part of the tile blocks.
 * @param tile
 * @param part
 * @param affector
 * @return amount of blockage
 */
int TerrainModifier::blockage(Tile *tile, const int part, Affector affector)
{
	int blockage = 0;
	if (tile == 0) return 0; // probably outside the map here
	if (tile->getMapData(part))
		blockage += tile->getMapData(part)->getBlock(affector);

	// open ufo doors are actually still closed behind the scenes
	// so a special trick is needed to see if they are open, if they are, they obviously don't block anything
	if (tile->isUfoDoorOpen(part)) 
		blockage = 0;

	if (tile->getMapData(O_OBJECT))
		blockage += tile->getMapData(O_OBJECT)->getBlock(affector);
	return blockage;
}

/**
 * The amount of power that is blocked going from one tile to another.
 * @param startTile
 * @param endTile
 * @param affector
 * @return amount of blockage
 */
int TerrainModifier::blockage(Tile *startTile, Tile *endTile, Affector affector)
{
	if (startTile == 0 || endTile == 0) return 0;

	int direction = vectorToDirection(endTile->getPosition() - startTile->getPosition());
	if (direction == -1) return 0;

	switch(direction)
	{
	case 0:	// north
		return blockage(startTile, O_NORTHWALL, affector);
		break;
	case 1: // north east
		return (blockage(startTile,O_NORTHWALL, affector) + blockage(endTile,O_WESTWALL, affector))/2
			+ (blockage(_save->getTile(startTile->getPosition() + Position(1, 0, 0)),O_WESTWALL, affector)
			+ blockage(_save->getTile(startTile->getPosition() + Position(1, 0, 0)),O_NORTHWALL, affector))/2;
		break;
	case 2: // east
		return blockage(endTile,O_WESTWALL, affector);
		break;
	case 3: // south east
		return (blockage(endTile,O_WESTWALL, affector) + blockage(endTile,O_NORTHWALL, affector))/2
			+ (blockage(_save->getTile(startTile->getPosition() + Position(1, 0, 0)),O_WESTWALL, affector)
			+ blockage(_save->getTile(startTile->getPosition() + Position(0, -1, 0)),O_NORTHWALL, affector))/2;
		break;
	case 4: // south
		return blockage(endTile,O_NORTHWALL, affector);
		break;
	case 5: // south west
		return (blockage(endTile,O_NORTHWALL, affector) + blockage(startTile,O_WESTWALL, affector))/2
			+ (blockage(_save->getTile(startTile->getPosition() + Position(0, -1, 0)),O_WESTWALL, affector)
			+ blockage(_save->getTile(startTile->getPosition() + Position(0, -1, 0)),O_NORTHWALL, affector))/2;
		break;
	case 6: // west
		return blockage(startTile,O_WESTWALL, affector);
		break;
	case 7: // north west
		return (blockage(startTile,O_WESTWALL, affector) + blockage(startTile,O_NORTHWALL, affector))/2
			+ (blockage(_save->getTile(startTile->getPosition() + Position(0, 1, 0)),O_WESTWALL, affector)
			+ blockage(_save->getTile(startTile->getPosition() + Position(-1, 0, 0)),O_NORTHWALL, affector))/2;
		break;
	}

	return false;
}

/*
 * Converts direction to a vector. Direction starts north = 0 and goes clockwise.
 * @param vector pointer to a position (which acts as a vector)
 * @return direction
 */
int TerrainModifier::vectorToDirection(const Position &vector)
{
	int x[8] = {0, 1, 1, 1, 0, -1, -1, -1};
	int y[8] = {1, 1, 0, -1, -1, -1, 0, 1};
	for (int i=0;i<9;i++)
	{
		if (x[i] == vector.x && y[i] == vector.y)
			return i;
	}
	return -1;
}

}