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
#define CIMGUI_USE_SDL3
#define CIMGUI_USE_OPENGL3
#include <cimgui/cimgui.h>
#include <cimgui_impl.h>
#include <stdlib.h>
#include <SDL3/SDL_platform_defines.h>

#include "tools.h"
#include "m_menu.h"
#include "sounds.h"
#include "g_game.h"
#include "m_misc.h"
#include "doomdef.h"
#include "s_sound.h"
#include "m_shift.h"
#include "con_cvar.h"
#include "st_stuff.h"
#include "doomstat.h"
#include "i_system.h"
#include "i_sdlinput.h"

/* Menu Externals */
MENU_EXTERNAL(menu_mainMenu);
MENU_EXTERNAL(menu_pause);

/* Variables */
boolean allowMenu = true;
boolean menuActive = false;
boolean mainMenuActive = false;
boolean allowClearMenu = true;

int map = 1;

/* Static Variables */
static boolean shiftDown = false;

static menu_t *currentMenu = NULL;
static menu_t *previousMenu = NULL;

/* Functions */
boolean M_Responder(event_t *event) {
	/* Taken and modified from the original menu code */
	int ch = -1;
	
	if (!allowMenu || demoplayback) {
		return false;
	}
	
	if (event->type == ev_mousedown) {
		if (event->data1 & 1) {
			ch = KEY_ENTER;
		}
		
		if (event->data1 & 4) {
			ch = KEY_BACKSPACE;
		}
	} else if (event->type == ev_keydown) {
		ch = event->data1;
		
		if (ch == KEY_SHIFT) {
			shiftDown = true;
		}
	} else if (event->type == ev_keyup) {
		if (event->data1 == KEY_SHIFT) {
			ch = event->data1;
			shiftDown = false;
		}
	}
	
	if (ch == KEY_ESCAPE) {
		if (menuActive) {
			M_SetupPrevMenu();
			return true;
		} else {
			if (!st_chatOn) {
				M_StartControlPanel();
				return true;
			}
		}
		return false;
	}
	return false;
}

void M_SetupMenu(
	menu_t *newMenu,
	boolean noPrev
) {
	if (newMenu != NULL) {
		if (
			currentMenu != NULL &&
			!noPrev
		) {
			previousMenu = currentMenu;
		}
		
		if (newMenu->init != NULL) {
			newMenu->init();
		}
		
		currentMenu = newMenu;
	}
	return;
}

void M_SetupPrevMenu() {
	if (
		currentMenu->previous != NULL ||
		currentMenu->autoPrev
	) {
		if (previousMenu == NULL) {
			M_ClearMenus();
		} else if (currentMenu->previous == NULL) {
			M_SetupMenu(previousMenu, false);
		} else {
			M_SetupMenu(currentMenu->previous, false);
		}
	}
	return;
}

void M_EndGame() {
	M_ClearMenus();
	gameaction = ga_title;
	M_SetupMenu(&menu_mainMenu, false);
	menuActive = true;
	return;
}

void M_StartControlPanel() {
	/* Taken and modified from the original menu code */
	if  (
		!allowMenu ||
		demoplayback ||
		menuActive
	) {
		return;
	}
	
	menuActive = true;
	
	previousMenu = NULL;
	currentMenu = usergame ? &menu_pause : &menu_mainMenu;
	
	S_PauseSound();
	return;
}

void M_StartMainMenu() {
	menuActive = true;
	M_SetupMenu(&menu_mainMenu, false);
	return;
}

void M_Ticker() {
	/* Taken and modified from the original menu code */
	mainMenuActive = currentMenu == &menu_mainMenu;
	
	if (
		(
			currentMenu == &menu_mainMenu ||
			currentMenu == &menu_pause
		) &&
		usergame &&
		demoplayback
	) {
		menuActive = false;
		return;
	}
	return;
}

void M_Drawer() {
	igSetNextWindowPos(
		(ImVec2){ 0.0f, 0.0f },
		ImGuiCond_FirstUseEver,
		(ImVec2){ 0.0f, 0.0f }
	);
	igSetNextWindowSize(
		igGetIO_Nil()->DisplaySize,
		ImGuiCond_FirstUseEver
	);
	
	igBegin(
		currentMenu->title,
		NULL,
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoBackground
	);
	
	if (currentMenu != NULL) {
		igText("%s", currentMenu->title);
		
		currentMenu->render();
	}
	
	igEnd();
	return;
}

void M_ClearMenus() {
	/* Taken and modified from the original menu code */
	if (!allowClearMenu) {
		return;
	}
	
	/*
		center mouse before clearing menu
		so the input code won't try to
		re-center the mouse; which can
		cause the player's view to warp
	*/
	if (gamestate == GS_LEVEL) {
		I_CenterMouse();
	}
	
	menuActive = false;
	
	currentMenu = previousMenu = NULL;
	
	S_ResumeSound();
	return;
}

void M_RestartLevel() {
	/* Taken and modified from the original menu code */
	if (!netgame) {
		gameaction = ga_loadlevel;
		nextmap = gamemap;
		players[consoleplayer].playerstate = PST_REBORN;
	}
	
	M_ClearMenus();
	return;
}

void M_Init() {
	M_ClearMenus();
	M_SetupMenu(&menu_mainMenu, false);
	menuActive = true;
	
	M_InitShiftXForm();
	return;
}