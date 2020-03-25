/*
 * sound.c
 *
 *  Created on: 30 jul. 2017
 *      Author: Ludo
 */

#include "sound.h"

#include "fmod.h"
#include "stdio.h"

/*** Sound local global variables ***/

static FMOD_SYSTEM* fmodSystem;

/*** SOUND functions ***/

/* INITIALISE FMOD SOUND SYSTEM.
 * @param:	None.
 * @return:	None.
 */
void SOUND_FmodSystemInit() {
	FMOD_System_Create(&fmodSystem);
	FMOD_System_Init(fmodSystem, 32, FMOD_INIT_NORMAL, NULL);
}

/* CREATE AN FMOD SOUND.
 * @param sound_ctx:	Pointer to sound structure.
 * @param wavPath:		Absolute or relative path of the sound file.
 * @param maxVolume:	Maximum normalized volume of the sound. Should be between 0.0 and 1.0.
 * @return : none.
 */
void SOUND_Create(SOUND_Context* sound_ctx, const char* audio_file_path, float maximum_volume) {
	// Structure initialisation.
	sound_ctx -> sound_maximum_volume = maximum_volume;
	sound_ctx -> sound_current_volume = 0.0;
	sound_ctx -> sound_length_ms = 0;
	sound_ctx -> sound_position_ms = 0;
	sound_ctx -> sound_is_playing = 0;
	FMOD_System_CreateSound(fmodSystem, audio_file_path, FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &((*sound_ctx).sound_fmod_sound));
	FMOD_Sound_GetLength((*sound_ctx).sound_fmod_sound, &((*sound_ctx).sound_length_ms), FMOD_TIMEUNIT_MS);
	printf("SOUND *** Audio file %s successfully opened.\n", audio_file_path);
}

/* PLAY A SOUND.
 * @param sound_ctx:	Sound to play.
 * @return: 			None.
 */
void SOUND_Play(SOUND_Context* sound_ctx) {
	FMOD_System_PlaySound(fmodSystem, FMOD_CHANNEL_FREE, ((*sound_ctx).sound_fmod_sound), 0, &((*sound_ctx).sound_fmod_channel));
}
