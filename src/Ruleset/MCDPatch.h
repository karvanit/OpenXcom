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
#ifndef OPENXCOM_MCDPATCH_H
#define OPENXCOM_MCDPATCH_H

#include <string>
#include <yaml-cpp/yaml.h>

namespace OpenXcom
{
class Ruleset;
class MapDataSet;
/**
 * An MCD data Patch.
 */
class MCDPatch
{
private:
	std::vector<std::pair<size_t, int> > _bigWalls, _TUWalks, _TUFlys, _TUSlides, _deathTiles;
public:
	/// Creates an MCD Patch.
	MCDPatch();
	/// Cleans up the MCD Patch.
	~MCDPatch();
	/// Loads the MCD Patch from YAML.
	void load(const YAML::Node& node, Ruleset *rules);
	/// Saves the MCD Patch to YAML.
	void save(YAML::Emitter& out) const;
	/// Applies an MCD patch to a mapDataSet
	void modifyData(MapDataSet *dataSet) const;
};

}

#endif
