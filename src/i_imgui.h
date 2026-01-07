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

#ifndef __I_IMGUI_H__
#define __I_IMGUI_H__

#include <SDL3/SDL.h>

int imgui_init(
	SDL_GLContext *glContext,
	SDL_Window *window
);
void imgui_event(SDL_Event *event);
void imgui_start();
void imgui_startWindow(const char *title);
void imgui_endWindow();
void imgui_render();
void imgui_uninit();

#endif