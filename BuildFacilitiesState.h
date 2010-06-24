/*
 * Copyright 2010 Daniel Albano
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
#ifndef OPENXCOM__BUILDFACILITIESSTATE_H
#define OPENXCOM__BUILDFACILITIESSTATE_H

#include "State_Interactive.h"
#include "LangString.h"
#include "Palette.h"
#include "Button.h"
#include "Window.h"
#include "Text.h"
#include "TextList.h"
#include "PlaceFacilityState.h"

class Base;

/**
 * Window shown wih all the facilities
 * available to build.
 */
class BuildFacilitiesState : public State
{
private:
	Base *_base;
	State *_state;

	Button *_btnOk;
	Window *_window;
	Text *_txtTitle;
	TextList *_lstFacilities;
public:
	/// Creates the Build Facilities state.
	BuildFacilitiesState(Game *game, Base *base, State *state);
	/// Cleans up the Build Facilities state.
	~BuildFacilitiesState();
	/// Updates the base stats.
	void init();
	/// Handler for clicking the OK button.
	void btnOkClick(SDL_Event *ev, int scale);
	/// Handler for clicking the Facilities list.
	void lstFacilitiesClick(SDL_Event *ev, int scale);
};

#endif