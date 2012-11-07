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
#ifndef OPENXCOM_WEIGHTEDOPTIONS_H
#define OPENXCOM_WEIGHTEDOPTIONS_H

#include <string>
#include <map>
#include <yaml-cpp/yaml.h>


namespace OpenXcom
{

/**
 * Holds pairs of relative weights and IDs.
 * It is used to store options and make a random choice between them.
 */
class WeightedOptions
{
public:
	/// Create an empty set.
	WeightedOptions() : _totalWeight(0) { /* Empty by design. */ }
	/// Select from among the items.
	const std::string &choose() const;
	/// Set an option's weight.
	void set(const std::string &id, unsigned weight);
	/// Is this empty?
	bool empty() const { return 0 == _totalWeight; }
	/// Remove all entries.
	void clear() { _totalWeight = 0; _choices.clear(); }
private:
	std::map<std::string, unsigned> _choices;
	unsigned _totalWeight;
friend void operator>>(const YAML::Node &, WeightedOptions &);
friend YAML::Emitter &operator<<(YAML::Emitter &, const WeightedOptions &);
};

}
#endif
