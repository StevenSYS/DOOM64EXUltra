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

#include "tools.h"
#include "m_menu.h"
#include "z_zone.h"
#include "p_setup.h"

#define LENGTH_NAMES 64

MENU_EXTERNAL(menu_mainMenu);
MENU_EXTERNAL(menu_skill);

extern int map;

typedef struct {
	char name[LENGTH_NAMES];
	char alphaKey;
} episode_t;

static unsigned int i;
static unsigned int episodeCount = 0;

static episode_t *episodes;

static void chooseEpisode(unsigned int episode) {
	/* Taken and modified from the original menu code */
	map = P_GetEpisode((int)episode)->mapid;
	M_SetupMenu(&menu_skill, false);
	return;
}

MENU_INIT {
	/* Taken and modified from the original menu code */
	episodeCount = P_GetNumEpisodes();
	episodedef_t *episode;
	
	if (episodeCount <= 0) {
		return;
	}
	
	episodes = Z_Realloc(
		episodes,
		sizeof(episode_t[episodeCount]),
		PU_STATIC,
		0
	);
	
	for (i = 0; i < episodeCount; i++) {
		episode = P_GetEpisode(i);
		strncpy(
			episodes[i].name,
			episode->name,
			LENGTH_NAMES
		);
		episodes[i].alphaKey = episode->key; // Not really needed
	}
	return;
}

MENU_RENDER {
	for (i = 0; i < episodeCount; i++) {
		if (igButton(episodes[i].name, (ImVec2){ 0.0f, 0.0f })) {
			chooseEpisode(i);
		}
	}
	return;
}

MENU_VAR(menu_episode, "Episode Select", &menu_mainMenu, false);