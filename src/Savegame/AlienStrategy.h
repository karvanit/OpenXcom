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
#ifndef OPENXCOM_ALIENSTRATEGY_H
#define OPENXCOM_ALIENSTRATEGY_H

#include <yaml-cpp/yaml.h>
#include "WeightedOptions.h"

namespace OpenXcom
{
class Ruleset;

/**
 * Stores the information about alien strategy.
 * This is a POD (plain old data) that only GeoscapeAI should touch directly.
 * The SavedGame should only use the YAML related parts, and constructor and
 * destructor.
 */
class AlienStrategy
{
public:
	/// Create an AlienStrategy with default data.
	AlienStrategy();
	/// Free resources used by the AlienStrategy.
	~AlienStrategy();
	/// Loads the data from YAML.
	void load(const Ruleset *rules, const YAML::Node& node);
	/// Saves the data to YAML.
	void save(YAML::Emitter& out) const;

	/* ONLY GeoscapeAI should access these */
	/// The chances of each region to be targeted for a mission.
	WeightedOptions _regionChances;
	/// The chances of each mission type for each region.
	std::map<std::string, WeightedOptions*> _regionMissions;

private:
	// Disable copy and assignments.
	AlienStrategy(const AlienStrategy &);
	AlienStrategy &operator=(const AlienStrategy &);
};

}

#endif
