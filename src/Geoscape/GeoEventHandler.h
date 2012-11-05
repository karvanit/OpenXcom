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
#ifndef OPENXCOM_GEOEVENTHANDLER_H
#define OPENXCOM_GEOEVENTHANDLER_H

namespace OpenXcom
{
// Forward declaration of ALL the event classes.
class GeoEventBase;
class Every30Minutes;
class EveryMonth;

/**
 * The GeoEventHandler interface.
 * It provides default handler methods that do nothing for all events.
 * There must be a handle() method for every event class.
 */
class GeoEventHandler
{
public:
	/// Allow proper destruction of derived instances.
	virtual ~GeoEventHandler() { /* Empty by design. */ }

protected:
	/// Handle unknown events.
	virtual void handle(const GeoEventBase &) { /* Do nothing. */ }
	/// Send every 30 game minutes.
	virtual void handle(const Every30Minutes &) { /* Do nothing. */ }
	/// Send after every game month.
	virtual void handle(const EveryMonth &) { /* Do nothing. */ }

template <class D> friend class GeoEvent;
};

}

#endif
