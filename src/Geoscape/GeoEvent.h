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
#ifndef OPENXCOM_GEOEVENT_H
#define OPENXCOM_GEOEVENT_H

#include "GeoEventHandler.h"

namespace OpenXcom
{
class GeoEventHandler;

/**
 * Abstract base for GeoEvent objects.
 * This is used to manage all events without caring for their actual
 * class.
 *
 * This is not to be used by other classes, derive from GeoEvent<YourClass>
 * instead.
 */
class GeoEventBase
{
public:
	/// Allow proper destruction of derived instances.
	virtual ~GeoEventBase() { /* Empty by design. */ }
	/// Call the proper handler method.
	virtual void inform(GeoEventHandler *handler) const = 0;
private:
	GeoEventBase() { /* Empty by design. */ }
	GeoEventBase(const GeoEventBase &);
	GeoEventBase & operator=(const GeoEventBase &);

template <class D> friend class GeoEvent;
};

/**
 * All GeoEvent classes must derive from this template.
 * It takes care of calling the appropriate event handler.
 */
template <class Derived>
class GeoEvent: public GeoEventBase
{
public:
	/**
	 * Call the proper handler method, based on type @a Derived.
	 * Do NOT override or reimplement this method.
	 */
	virtual void inform(GeoEventHandler *handler) const
	{
		handler->handle(*static_cast<const Derived *>(this));
	}
protected:
	GeoEvent() : GeoEventBase() { /* Empty by design. */ }
};

}

#endif
