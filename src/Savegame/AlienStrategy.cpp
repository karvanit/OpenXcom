/*
 * Copyright 2012 OpenXcom Developers.
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
#include "AlienStrategy.h"
#include "SavedGame.h"
#include "WeightedOptions.h"
#include "../Engine/RNG.h"
#include "../Ruleset/Ruleset.h"

namespace OpenXcom
{

typedef std::map<std::string, WeightedOptions*> MissionsByRegion;

/**
 * Create an AlienStrategy with no values.
 * Running a game like this will most likely crash.
 */
AlienStrategy::AlienStrategy()
{
	// Empty by design!
}

/**
 * Free all resources used by this AlienStrategy.
 */
AlienStrategy::~AlienStrategy()
{
	// Free allocated memory.
	for (MissionsByRegion::iterator ii = _regionMissions.begin(); ii != _regionMissions.end(); ++ii)
	{
		delete ii->second;
	}
}

/**
 * Loads the data from a YAML file.
 * @param node YAML node.
 */
void AlienStrategy::load(const Ruleset *rules, const YAML::Node &node)
{
	// Free allocated memory.
	for (MissionsByRegion::iterator ii = _regionMissions.begin(); ii != _regionMissions.end(); ++ii)
	{
		delete ii->second;
	}
	_regionMissions.clear();
	_regionChances.clear();
	node["regions"] >> _regionChances;
	const YAML::Node &strat = node["possibleMissions"];
	for (YAML::Iterator nn = strat.begin(); nn != strat.end(); ++nn)
	{
		std::string region;
		(*nn)["region"] >> region;
		const YAML::Node &missions = (*nn)["missions"];
		std::auto_ptr<WeightedOptions> options(new WeightedOptions());
		missions >> *options;
		_regionMissions.insert(std::make_pair(region, options.release())); 
	}
}

/**
 * Saves the alien data to a YAML file.
 * @param out YAML emitter.
 */
void AlienStrategy::save(YAML::Emitter &out) const
{
	out << YAML::BeginMap;
	out << YAML::Key << "regions" << YAML::Value << _regionChances;
	out << YAML::Key << "possibleMissions" << YAML::Value;
	out << YAML::BeginSeq;
	for (MissionsByRegion::const_iterator ii = _regionMissions.begin(); ii != _regionMissions.end(); ++ii)
	{
		out << YAML::BeginMap;
		out << YAML::Key << ii->first << YAML::Value << *ii->second;
		out << YAML::EndMap;
	}
	out << YAML::EndSeq;
	out << YAML::EndMap;
}

}
