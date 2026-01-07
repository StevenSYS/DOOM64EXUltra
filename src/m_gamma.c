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

#include "m_menu.h"
#include "d_englsh.h"
#include "doomstat.h"
#include "con_cvar.h"

CVAR_EXTERNAL(i_gamma);

static char gammamsg[21][28] = {
	GAMMALVL0,
	GAMMALVL1,
	GAMMALVL2,
	GAMMALVL3,
	GAMMALVL4,
	GAMMALVL5,
	GAMMALVL6,
	GAMMALVL7,
	GAMMALVL8,
	GAMMALVL9,
	GAMMALVL10,
	GAMMALVL11,
	GAMMALVL12,
	GAMMALVL13,
	GAMMALVL14,
	GAMMALVL15,
	GAMMALVL16,
	GAMMALVL17,
	GAMMALVL18,
	GAMMALVL19,
	GAMMALVL20
};

void M_ChangeGammaLevel(int choice) {
	switch (choice) {
		case 0:
			if (i_gamma.value > 0.0f) {
				M_SetCvar(&i_gamma, i_gamma.value - 1);
			}
			else {
				CON_CvarSetValue(i_gamma.name, 0);
			}
			break;
		case 1:
			if (i_gamma.value < 20.0f) {
				M_SetCvar(&i_gamma, i_gamma.value + 1);
			}
			else {
				CON_CvarSetValue(i_gamma.name, 20);
			}
			break;
		case 2:
			if (i_gamma.value >= 20) {
				CON_CvarSetValue(i_gamma.name, 0);
			}
			else {
				CON_CvarSetValue(i_gamma.name, i_gamma.value + 1);
			}
			
			players[consoleplayer].message = gammamsg[(int)i_gamma.value];
			break;
		default:
			break;
	}
	return;
}