// Emacs style mode select   -*- C -*-
//-----------------------------------------------------------------------------
//
// Copyright(C) 2024-2025 Gibbon
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
//
// DESCRIPTION: Low-level audio API. Incorporates a sequencer system to
//              handle all sounds and music. All code related to the sequencer
//              is kept in it's own module and seperated from the rest of the
//              game code.
//
//-----------------------------------------------------------------------------

#include "tools.h"
#include "w_wad.h"
#include "z_zone.h"
#include "doomdef.h"
#include "i_audio.h"
#include "i_system.h"
#include "con_console.h"

/* Variables */
int sfxCount = 0;

/* Static Variables */
static int *musLumpIndex;
static int *sfxLumpIndex;

static struct Sound sound;

/* Static Functions */
static int registerSounds() {
	int i, successCount = 0, failCount = 0;
	int len = 0;
	unsigned char *data;
	SDL_IOStream *ioStream;
	MIX_Audio *audio;
	
	memset(
		sound.sounds,
		0,
		MAX_GAME_SFX * sizeof(MIX_Audio *)
	);
	sfxCount = 0;
	
	sfxLumpIndex = (int *)Z_Realloc(
		sfxLumpIndex,
		sizeof(int) * numlumps,
		PU_STATIC,
		0
	);
	
	for (i = 0; i < numlumps; i++) {	
		sfxLumpIndex[i] = -1;
	}
	
	for (i = 0; i < numlumps; i++) {
		if (sfxCount >= MAX_GAME_SFX) {
			CON_Warnf(
				"registerSounds: Exceeded limit of %d. Further SFX ignored.\n",
				MAX_GAME_SFX
			);
			failCount += (numlumps - 1);
			break;
		}
		
		len = W_LumpLength(i);
		
		if (len < 4) {
			continue;
		}
		
		data = (unsigned char *)W_CacheLumpNum(i, PU_STATIC);
		
		if (data == NULL) {
			failCount++;
			continue;
		}
		
		if (
			dstrncmp((char *)data, "RIFF", 4) != 0 &&
			dstrncmp((char *)data + 8, "WAVE", 4) != 0 &&
			dstrncmp((char *)data, "FORM", 4) != 0 &&
			dstrncmp((char *)data + 8, "AIFF", 4) != 0 &&
			dstrncmp((char *)data + 8, "AIFC", 4) != 0 &&
			dstrncmp((char *)data, "OggS", 4) != 0 &&
			dstrncmp((char *)data, "fLaC", 4) != 0 &&
			dstrncmp((char *)data, "FSB5", 4) != 0 &&
			dstrncmp((char *)data, "FSB4", 4) != 0 &&
			dstrncmp((char *)data, "ID3", 4) != 0
		) {
			continue;
		}
		
		ioStream = SDL_IOFromMem(data, len);
		
		audio = MIX_LoadAudio_IO(
			sound.mixer,
			ioStream,
			true,
			true
		);
		
		if (audio == NULL) {
			failCount++;
		} else {
			I_Printf("------%d\n", sfxCount);
			sound.sounds[sfxCount] = audio;
			sfxLumpIndex[i] = sfxCount;
			successCount++;
		}
		
		sfxCount++;
	}
	
	if (failCount > 0) {
		I_Printf("Failed to load %d sound effects.\n", failCount);
	}
	
	if (successCount > 0) {
		I_Printf("Succsesfully registered %d sound effects.\n", successCount);
	}
	return 0;
}

/* Functions */
int I_GetMaxChannels() {
	STUB();
	return 0;
}

int I_GetVoiceCount() {
	STUB();
	return 0;
}

sndsrc_t *I_GetSoundSource(int c) {
	STUB();
	return 0;
}

void Chan_SetMusicVolume(float volume) {
	STUB();
	return;
}

void Chan_SetSoundVolume(float volume) {
	STUB();
	return;
}

void Seq_SetGain(float db) {
	STUB();
	return;
}

void FMOD_CreateMusicTracksInit() {
	STUB();
	return;
}

void FMOD_CreateSfxTracksInit() {
	STUB();
	return;
}

int FMOD_StartSound(int sfx_id, sndsrc_t *origin, int volume, int pan) {
	INCOMPLETE();
	
	if (sound.mixer == NULL) {
		return -1;
	}
	
	if (
		sfx_id < 0 ||
		sfx_id >= sfxCount ||
		!sound.sounds[sfx_id]
	) {
		CON_Warnf("FMOD_StartSound: Invalid sfx_id %d or sound not loaded (num_sfx: %s)\n", sfx_id, sfxCount);
		return -1;
	}
	
	if (!MIX_PlayAudio(sound.mixer, sound.sounds[sfx_id])) {
		I_Printf("FMOD_StartSound: %s", SDL_GetError());
		return -1;
	}
	return 0;
}

int FMOD_StartSoundPlasma(int sfx_id) {
	STUB();
	return 0;
}

void FMOD_StopSound(sndsrc_t *origin, int sfx_id) {
	STUB();
	return;
}

int FMOD_StartMusic(int mus_id) {
	STUB();
	return 0;
}

void FMOD_StopMusic(sndsrc_t *origin, int mus_id) {
	STUB();
	return;
}

int FMOD_StartSFXLoop(int sfx_id, int volume) {
	STUB();
	return 0;
}

int FMOD_StopSFXLoop() {
	STUB();
	return 0;
}

int FMOD_StartPlasmaLoop(int sfx_id, int volume) {
	STUB();
	return 0;
}

void FMOD_StopPlasmaLoop() {
	STUB();
	return;
}

void FMOD_PauseMusic() {
	STUB();
	return;
}

void FMOD_ResumeMusic() {
	STUB();
	return;
}

void FMOD_PauseSFXLoop() {
	STUB();
	return;
}

void FMOD_ResumeSFXLoop() {
	STUB();
	return;
}

void I_InitSequencer() {
	INCOMPLETE();
	
	if (!MIX_Init()) {
		I_Error("I_InitSeqencer: Failed to initalize SDL3_mixer: %s\n", SDL_GetError());
		return;
	}
	
	sound.mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, sound.spec);
	
	if (sound.mixer == NULL) {
		I_Error("I_InitSeqencer: Failed to initalize mixer: %s\n", SDL_GetError());
		return;
	}
	
	if (registerSounds()) {
		return;
	}
	return;
}

void I_ShutdownSound() {
	unsigned int i;
	
	INCOMPLETE();
	
	MIX_DestroyMixer(sound.mixer);
	MIX_Quit();
	
	for (i = 0; i < MAX_GAME_SFX; i++) {
		MIX_DestroyAudio(sound.sounds[i]);
	}
	
	Z_Free(sfxLumpIndex);
	return;
}

void I_Update() {
	STUB();
	return;
}

void I_UpdateChannel(int c, int volume, int pan, fixed_t x, fixed_t y) {
	STUB();
	return;
}

void I_RemoveSoundSource(int c) {
	STUB();
	return;
}

void I_SetMusicVolume(float volume) {
	STUB();
	return;
}

void I_SetSoundVolume(float volume) {
	MIX_SetMasterGain(
		sound.mixer,
		(float)volume / 255
	);
	return;
}

void I_ResetSound() {
	STUB();
	return;
}

void I_PauseSound() {
	STUB();
	return;
}

void I_ResumeSound() {
	STUB();
	return;
}

void I_SetGain(float db) {
	STUB();
	return;
}

void I_StopSound(sndsrc_t *origin, int sfx_id) {
	STUB();
	return;
}

void I_StartMusic(int mus_id) {
	STUB();
	return;
}

void I_UpdateListenerPosition(fixed_t player_world_x, fixed_t player_world_y_depth, fixed_t player_eye_world_z_height, angle_t view_angle) {
	STUB();
	return;
}