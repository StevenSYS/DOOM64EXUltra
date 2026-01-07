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
#include "g_game.h"

#define LENGTH_SKILLS 5

extern boolean allowMenu;

extern int map;

static const char *skills[LENGTH_SKILLS] = {
	"Be Gentle!",
	"Bring It On!",
	"I Own Doom!",
	"Watch Me Die!",
	"Hardcore!"
};

static unsigned int i;

MENU_EXTERNAL(episode);

static void M_ChooseSkill(int choice) {
	G_DeferedInitNew(choice, map);
	M_ClearMenus();
	allowMenu = false;
	return;
}

MENU_INIT {
	return;
}

MENU_RENDER {
	for (i = 0; i < LENGTH_SKILLS; i++) {
		if (igButton(skills[i], (ImVec2){ 0.0f, 0.0f })) {
			M_ChooseSkill(i);
		}
	}
	return;
}

MENU_VAR(skill, "Skill Select", true);