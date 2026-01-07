// Emacs style mode select   -*- C -*-
//-----------------------------------------------------------------------------
//
// Copyright(C) 2026 StevenSYS
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
//-----------------------------------------------------------------------------

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui/cimgui.h>

#include "m_menu.h"
#include "i_system.h"

MENU_INIT {
	return;
}

MENU_RENDER {
	if (igButton("Yes", (ImVec2){ 0.0f, 0.0f })) {
		I_Quit();
	}
	
	if (igButton("No", (ImVec2){ 0.0f, 0.0f })) {
		M_SetupPrevMenu();
	}
	return;
}

MENU_VAR(quitGame, "Quit DOOM 64?", true);