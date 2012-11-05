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
#ifndef OPENXCOM_GEOEVENTS_H
#define OPENXCOM_GEOEVENTS_H

#include "GeoEvent.h"

namespace OpenXcom
{

/**
 * This event is send every 30 game minutes.
 */
class Every30Minutes: public GeoEvent<Every30Minutes> { /* Empty by design */ };

/**
 * This event is send at the end of every game month.
 */
class EveryMonth: public GeoEvent<EveryMonth> { /* Empty by design */ };

}

#endif
