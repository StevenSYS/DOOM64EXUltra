// Emacs style mode select   -*- C -*-
//-----------------------------------------------------------------------------
//
// Copyright(C) 1993-1997 Id Software, Inc.
// Copyright(C) 2007-2012 Samuel Villarreal
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

#ifndef __M_MENU__
#define __M_MENU__

#include "d_event.h"
#include "con_cvar.h"

/* Macros */

/* Not Static */
#define MENU_NS_INIT(_name) void _name()
#define MENU_NS_RENDER(_name) void _name()

#define MENU_INIT static MENU_NS_INIT(init)
#define MENU_RENDER static MENU_NS_RENDER(render)

#define MENU_VAR(_vName, _name, _enablePrev) \
	menu_t menu_ ## _vName = { \
		_name, \
		NULL, \
		_enablePrev, \
		&init, \
		&render \
	}


#define MENU_EXTERNAL(_vName) \
	extern menu_t menu_ ## _vName

/* Typedef Functions */
typedef MENU_NS_INIT((menu_init_f));
typedef MENU_NS_RENDER((menu_render_f));

/* Structs */
typedef struct menu_s {
	const char *title;
	struct menu_s *prev;
	boolean enablePrev;
	
	/* Functions */
	menu_init_f *init;
	menu_render_f *render;
} menu_t;

/* Functions */
void M_SetCvar(
	cvar_t *cvar,
	float value
);

boolean M_Responder(event_t *event);

void M_SetupMenu(
	menu_t *next,
	boolean noPrev
);

void M_SetupPrevMenu();
void M_Ticker();
void M_Drawer();
void M_Init();
void M_StartControlPanel();
void M_StartMainMenu();
void M_ClearMenus();

#endif