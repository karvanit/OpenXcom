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
#include "RuleAlienMission.h"
#include "Ruleset.h"
#include "../Savegame/WeightedOptions.h"
#include "../Engine/RNG.h"
#include "../Savegame/GameTime.h"

namespace OpenXcom
{

void operator>>(const YAML::Node &nd, RuleAlienMission &mission)
{
	nd["type"] >> mission._type;
	//Only allow full replacement of mission racial distribution.
	if (const YAML::Node *pRaces = nd.FindValue("races"))
	{
		std::vector<std::pair<unsigned, WeightedOptions*> > nraces(pRaces->size());
		for (unsigned ii = 0; ii < pRaces->size(); ++ii)
		{
			(*pRaces)[ii]["afterMonth"] >> nraces[ii].first;
			std::auto_ptr<WeightedOptions> dist(new WeightedOptions());
			(*pRaces)[ii]["distribution"] >> *dist;
			nraces[ii].second = dist.release();
		}
		mission._raceDistribution.swap(nraces);
	}
}

YAML::Emitter &operator<<(YAML::Emitter &out, const RuleAlienMission &mission)
{
	out << YAML::BeginMap;
	out << YAML::Key << "type" << YAML::Value << mission._type;
	out << YAML::Key << "races" << YAML::Value;
	out << YAML::BeginSeq;
	for (std::vector<std::pair<unsigned, WeightedOptions*> >::const_iterator ii = mission._raceDistribution.begin();
	     ii != mission._raceDistribution.end(); ++ii)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "afterMonth" << YAML::Value << ii->first;
		out << YAML::Key << "distribution" << YAML::Value << *ii->second;
		out << YAML::EndMap;
	}
	out << YAML::EndSeq;
	return out;
}

/**
 * Choose one of the available races for this mission.
 * @param gameTime The current date and time of the game world.
 * @return The string id of the race.
 */
const std::string &RuleAlienMission::generateRace(const GameTime &gameTime) const
{
	unsigned months = (gameTime.getYear() - 1999) * 12 + gameTime.getMonth() - 1;
	assert(months >= _raceDistribution.front().first);
	std::vector<std::pair<unsigned, WeightedOptions*> >::const_reverse_iterator rc = _raceDistribution.rbegin();
	while (months < rc->first)
		++rc;
	return rc->second->choose();
}

}
