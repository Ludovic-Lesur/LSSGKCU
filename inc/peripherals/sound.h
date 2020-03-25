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
	float sound_maximum_volume;
	float sound_current_volume;
	unsigned int sound_length_ms;
	unsigned int sound_position_ms;
	FMOD_CHANNEL* sound_fmod_channel;
	FMOD_SOUND* sound_fmod_sound;
	FMOD_BOOL sound_is_playing;
} SOUND_Context;

/*** SOUND functions ***/

void SOUND_FmodSystemInit();
void SOUND_Create(SOUND_Context* sound_ctx, const char* audio_file_path, float maximum_volume);
void SOUND_Play(SOUND_Context* sound_ctx);

#endif /* SOUND_H */
