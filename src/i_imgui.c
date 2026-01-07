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
#include <SDL3/SDL.h>

#include "i_imgui.h"
#include "i_system.h"
#include "progInfo.h"

static char inited = 0;

int imgui_init(
	SDL_GLContext *glContext,
	SDL_Window *window
) {
	ImVec4_c *colors;
	
	igCreateContext(NULL);
	igStyleColorsDark(NULL);
	
	igGetIO_Nil()->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	igGetIO_Nil()->IniFilename = NULL;
	
	if (!ImGui_ImplSDL3_InitForOpenGL(window, glContext)) {
		I_Error("I_InitScreen: Failed to init ImGui");
		return 1;
	}
	
	if (!ImGui_ImplOpenGL3_Init(
		#if __APPLE__
		"#version 150"
		#else
		"#version 130"
		#endif
	)) {
		I_Error("I_InitScreen: Failed to init ImGui Renderer");
		return 1;
	}
	
	igSetCurrentFont(NULL, FONT_SIZE, FONT_SIZE);
	
	/* Background Color */
	colors = igGetStyle()->Colors;
	
	colors[ImGuiCol_WindowBg] = (ImVec4){
		0.0f, 0.0f, 0.0f,
		(float)0x80 / 255
	};
	
	inited = 1;
	return 0;
}

void imgui_event(SDL_Event *event) {
	if (inited) {
		ImGui_ImplSDL3_ProcessEvent(event);
	}
	return;
}

void imgui_start() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	igNewFrame();
	return;
}

void imgui_startWindow(const char *title) {
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
		title,
		NULL,
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoTitleBar
	);
	return;
}

void imgui_endWindow() {
	igEnd();
	return;
}

void imgui_render() {
	igRender();
	ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
	return;
}

void imgui_uninit() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	igDestroyContext(NULL);
	
	inited = 0;
	return;
}