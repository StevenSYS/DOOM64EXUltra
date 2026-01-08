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
#include "p_mobj.h"
#include "z_zone.h"
#include "doomdef.h"
#include "i_audio.h"
#include "i_system.h"
#include "con_console.h"

/* Macros */
#define INCHES_PER_METER 39.3701f

/* Calculate Left/Right Panning */
#define CLP(_pan) (float)(volume - (_pan - 128)) / 256
#define CRP(_pan) (float)(volume + (_pan - 128)) / 256

/* Variables */
int sfxCount = 0;

/* Static Variables */
static sound_t sound;

/* Static Functions */
static int registerSounds() {
	int i, successCount = 0, failCount = 0;
	int len = 0;
	unsigned char *data;
	SDL_IOStream *ioStream;
	MIX_Track *track;
	MIX_Audio *audio;
	
	memset(
		sound.sounds,
		0,
		MAX_GAME_SFX * sizeof(MIX_Audio *)
	);
	sfxCount = 0;
	
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
		
		track = MIX_CreateTrack(sound.mixer);
		
		audio = MIX_LoadAudio_IO(
			sound.mixer,
			ioStream,
			true,
			true
		);
		
		if (
			track == NULL ||
			audio == NULL
		) {
			failCount++;
		} else {
			MIX_SetTrackAudio(track, audio);
			sound.audio[sfxCount] = audio;
			sound.sounds[sfxCount] = track;
			successCount++;
		}
		
		MIX_DestroyAudio(audio);
		
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

static int startLoop(
	MIX_Track *track,
	int sfx_id,
	int volume
) {
	if (volume <= 0) {
		return 0;
	}
	
	if (sound.mixer == NULL) {
		return -1;
	}
	
	if (!MIX_SetTrackAudio(track, sound.audio[sfx_id])) {
		I_Printf("startLoop: MIX_SetTrackAudio: %s\n", SDL_GetError());
		return -1;
	}
	
	if (
		sfx_id < 0 ||
		sfx_id >= sfxCount ||
		sound.sounds[sfx_id] == NULL
	) {
		CON_Warnf("startLoop: Invalid sfx_id %d or sound not loaded (num_sfx: %s)\n", sfx_id, sfxCount);
		return -1;
	}
	
	if (!MIX_PlayTrack(track, 0)) {
		I_Printf("startLoop: MIX_PlayTrack: %s\n", SDL_GetError());
		return -1;
	}
	
	if (!MIX_SetTrackGain(
		track,
		(float)volume / 256
	)) {
		I_Printf("startLoop: MIX_SetTrackGain: %s\n", SDL_GetError());
		return -1;
	}
	
	if (!MIX_SetTrackLoops(track, -1)) {
		I_Printf("startLoop: MIX_SetTrackLoops: %s\n", SDL_GetError());
		return -1;
	}
	return 0;
}

static int stopLoop(MIX_Track *track) {
	if (
		sound.mixer == NULL ||
		!MIX_TrackLooping(track)
	) {
		return -1;
	}
	
	if (!MIX_StopTrack(track, 0)) {
		I_Printf("stopLoop: MIX_StopTrack: %s\n", SDL_GetError());
		return -1;
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

int FMOD_StartSound(
	int sfx_id,
	sndsrc_t *origin,
	int volume,
	int pan
) {
	float leftVol;
	float rightVol;
	
	if (volume <= 0) {
		return 0;
	}
	
	if (sound.mixer == NULL) {
		return -1;
	}
	
	if (
		sfx_id < 0 ||
		sfx_id >= sfxCount ||
		sound.sounds[sfx_id] == NULL
	) {
		CON_Warnf("FMOD_StartSound: Invalid sfx_id %d or sound not loaded (num_sfx: %s)\n", sfx_id, sfxCount);
		return -1;
	}
	
	leftVol = CLP(pan);
	rightVol = CRP(pan);
	
	if (leftVol > 1.0f) {
		leftVol = 1.0f;
	}
	
	if (rightVol > 1.0f) {
		rightVol = 1.0f;
	}
	
	if (!MIX_SetTrackStereo(
		sound.sounds[sfx_id],
		&(MIX_StereoGains){
			leftVol,
			rightVol
		}
	)) {
		I_Printf("FMOD_StartSound: MIX_SetTrackStereo: %s\n", SDL_GetError());
		return -1;
	}
	
	if (!MIX_PlayTrack(sound.sounds[sfx_id], 0)) {
		I_Printf("FMOD_StartSound: MIX_PlayTrack: %s\n", SDL_GetError());
		return -1;
	}
	return 0;
}

int FMOD_StartSoundPlasma(int sfx_id) {
	STUB();
	return 0;
}

void FMOD_StopSound(
	sndsrc_t *origin,
	int sfx_id
) {
	if (
		sound.mixer == NULL ||
		!MIX_TrackPlaying(sound.sounds[sfx_id])
	) {
		return;
	}
	
	if (!MIX_StopTrack(sound.sounds[sfx_id], 0)) {
		I_Printf("FMOD_StopSound: MIX_StopTrack: %s\n", SDL_GetError());
		return;
	}
	return;
}

int FMOD_StartMusic(int mus_id) {
	STUB();
	return 0;
}

void FMOD_StopMusic(
	sndsrc_t *origin,
	int mus_id
) {
	STUB();
	return;
}

int FMOD_StartSFXLoop(
	int sfx_id,
	int volume
) {
	return startLoop(sound.loop, sfx_id, volume);
}

int FMOD_StopSFXLoop() {
	return stopLoop(sound.loop);
}

int FMOD_StartPlasmaLoop(
	int sfx_id,
	int volume
) {
	return startLoop(sound.plasmaLoop, sfx_id, volume);
}

void FMOD_StopPlasmaLoop() {
	stopLoop(sound.plasmaLoop);
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
		I_Error("I_InitSeqencer: MIX_Init: %s\n", SDL_GetError());
		return;
	}
	
	sound.mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, sound.spec);
	
	if (sound.mixer == NULL) {
		I_Error("I_InitSeqencer: MIX_CreateMixerDevice: %s\n", SDL_GetError());
		return;
	}
	
	sound.loop = MIX_CreateTrack(sound.mixer);
	sound.plasmaLoop = MIX_CreateTrack(sound.mixer);
	
	if (registerSounds()) {
		return;
	}
	return;
}

void I_ShutdownSound() {
	unsigned int i;
	
	INCOMPLETE();
	
	for (i = 0; i < sfxCount; i++) {
		MIX_DestroyTrack(sound.sounds[i]);
	}
	
	MIX_DestroyMixer(sound.mixer);
	MIX_Quit();
	return;
}

void I_Update() {
	STUB();
	return;
}

void I_UpdateChannel(
	int c,
	int volume,
	int pan,
	fixed_t x, fixed_t y
) {
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
		volume / 255.0f
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

void I_StopSound(
	sndsrc_t *origin,
	int sfx_id
) {
	STUB();
	return;
}

void I_StartMusic(int mus_id) {
	STUB();
	return;
}