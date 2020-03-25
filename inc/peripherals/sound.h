/*
 * sound.h
 *
 *  Created on: 25 mar. 2020
 *      Author: Ludo
 */

#ifndef SOUND_H
#define SOUND_H

#include "fmod.h"

/*** SOUND structure ***/

typedef struct Sound_Struct {
	FMOD_CHANNEL* sound_fmod_channel;
	FMOD_SOUND* sound_fmod_sound;
	unsigned int sound_length_ms;
	float sound_current_volume;
	float sound_maximum_volume;
	float sound_fade_start_volume;
	unsigned int sound_fade_start_position_ms;
} SOUND_Context;

/*** SOUND functions ***/

void SOUND_FmodSystemInit();
void SOUND_Init(SOUND_Context* sound_ctx, const char* audio_file_path, float maximum_volume);
void SOUND_Play(SOUND_Context* sound_ctx);
void SOUND_Stop(SOUND_Context* sound_ctx);
void SOUND_SetVolume(SOUND_Context* sound_ctx, float new_volume);
unsigned int SOUND_GetLengthMs(SOUND_Context* sound_ctx);
unsigned int SOUND_GetPositionMs(SOUND_Context* sound_ctx);
void SOUND_SetPosition(SOUND_Context* sound_ctx, unsigned int new_position_ms);
FMOD_BOOL SOUND_IsPlaying(SOUND_Context* sound_ctx);
void SOUND_SaveFadeParameters(SOUND_Context* sound_ctx);
unsigned char SOUND_FadeIn(SOUND_Context* sound_ctx, unsigned int fade_duration_ms);
unsigned char SOUND_FadeOut(SOUND_Context* sound_ctx, unsigned int fade_duration_ms);

#endif /* SOUND_H */
