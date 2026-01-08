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
#include "i_audio.h"

CVAR_EXTERNAL(s_gain);
CVAR_EXTERNAL(s_sfxvol);
CVAR_EXTERNAL(s_musvol);

MENU_INIT {
	return;
}

MENU_RENDER {
	if (igSliderFloat(
		"##masterVol",
		&s_gain.value,
		0.0f,
		255.0f,
		"Master Volume",
		ImGuiSliderFlags_None
	)) {
		I_SetGain(s_gain.value);
	}
	
	if (igSliderFloat(
		"##sndVol",
		&s_sfxvol.value,
		0.0f,
		255.0f,
		"Sound Volume",
		ImGuiSliderFlags_None
	)) {
		I_SetSoundVolume(s_sfxvol.value);
	}
	return;
}

MENU_VAR(volume, "Volume Control", true);