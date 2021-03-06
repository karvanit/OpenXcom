/*
 * Copyright 2010-2013 OpenXcom Developers.
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
#include "MapBlock.h"

namespace OpenXcom
{

/**
* MapBlock construction
*/
MapBlock::MapBlock(RuleTerrain *terrain, std::string name, int size_x, int size_y, MapBlockType type):_terrain(terrain), _name(name), _size_x(size_x), _size_y(size_y), _size_z(0), _type(type), _subType(MT_UNDEFINED), _frequency(1), _timesUsed(0), _maxCount(-1)
{
}

/**
* MapBlock DESTRUCTION.
*/
MapBlock::~MapBlock()
{
}

/**
 * Loads the map block from a YAML file.
 * @param node YAML node.
 */
void MapBlock::load(const YAML::Node &node)
{
	for (YAML::Iterator i = node.begin(); i != node.end(); ++i)
	{
		std::string key;
		i.first() >> key;
		if (key == "name")
		{
			i.second() >> _name;
		}
		else if (key == "width")
		{
			i.second() >> _size_x;
		}
		else if (key == "length")
		{
			i.second() >> _size_y;
		}
		else if (key == "height")
		{
			i.second() >> _size_z;
		}
		else if (key == "type")
		{
			int a;
			i.second() >> a;
			_type = (MapBlockType)a;
			if (_subType == MT_UNDEFINED)
				_subType = (MapBlockType)a;
		}
		else if (key == "subType")
		{
			int a;
			i.second() >> a;
			_subType = (MapBlockType)a;
		}
		else if (key == "frequency")
		{
			i.second() >> _frequency;
		}
		else if (key == "maxCount")
		{
			i.second() >> _maxCount;
		}
	}
}

/**
 * Saves the map block to a YAML file.
 * @param out YAML emitter.
 */
void MapBlock::save(YAML::Emitter &out) const
{
	out << YAML::BeginMap;
	out << YAML::Key << "name" << YAML::Value << _name;
	out << YAML::Key << "width" << YAML::Value << _size_x;
	out << YAML::Key << "length" << YAML::Value << _size_y;
	out << YAML::Key << "height" << YAML::Value << _size_z;
	out << YAML::Key << "type" << YAML::Value << (int)_type;
	out << YAML::Key << "subType" << YAML::Value << (int)_subType;
	out << YAML::Key << "frequency" << YAML::Value << _frequency;
	out << YAML::Key << "maxCount" << YAML::Value << _maxCount;
	out << YAML::EndMap;
}

/**
* Gets the MapBlock name (string).
* @return name
*/
std::string MapBlock::getName() const
{
	return _name;
}

/**
* Gets the MapBlock size x.
* @return size x in tiles.
*/
int MapBlock::getSizeX() const
{
	return _size_x;
}

/**
* Gets the MapBlock size y.
* @return size y in tiles.
*/
int MapBlock::getSizeY() const
{
	return _size_y;
}

/**
* Sets the MapBlock size z.
* @param size z
*/
void MapBlock::setSizeZ(int size_z)
{
	_size_z = size_z;
}

/**
* Gets the MapBlock size z.
* @return size z
*/
int MapBlock::getSizeZ() const
{
	return _size_z;
}

/**
* Get the type of mapblock.
* @return type
*/
MapBlockType MapBlock::getType() const
{
	return _type;
}

MapBlockType MapBlock::getSubType() const
{
	return _subType;
}

int MapBlock::getRemainingUses()
{
	if (_maxCount == -1)
	{
		return _frequency;
	}
	return _maxCount - _timesUsed;
}

void MapBlock::markUsed()
{
	if (_maxCount == -1)
	{
		return;
	}
	_timesUsed++;
	if (_timesUsed >= _maxCount)
	{
		_timesUsed = _maxCount;
	}
}

void MapBlock::reset()
{
	_timesUsed = 0;
}

}
