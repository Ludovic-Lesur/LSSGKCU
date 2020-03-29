/*
 * zdj.c
 *
 *  Created on: 26 mar. 2020
 *      Author: Ludo
 */

#include "zdj.h"

#include "mixer.h"
#include "sound.h"
#include "stdio.h"

/*** ZDJ local global variables ***/

static SOUND_Context zdj_open_sound;
static SOUND_Context zdj_lock_sound;

/*** ZDJ functions ***/

/* INIT ZDJ SOUND MODULE.
 * @param:	None.
 * @return:	None.
 */
void ZDJ_Init(void) {
	// Init sound.
	SOUND_Init(&zdj_open_sound, "zdj.wav", ZDJ_AUDIO_GAIN);
	SOUND_SetVolume(&zdj_open_sound, 1.0); // No fade effect required.
	SOUND_Init(&zdj_lock_sound, "zen.wav", ZDJ_AUDIO_GAIN);
	SOUND_SetVolume(&zdj_lock_sound, 1.0); // No fade effect required.
}

/* OPEN CIRCUIT BREAKER.
 * @param:	None.
 * @return:	None.
 */
void ZDJ_Open(void) {
	// Play sound.
	SOUND_Play(&zdj_open_sound);
	SOUND_Stop(&zdj_lock_sound);
	printf("ZDJ *** Open.\n");
	fflush(stdout);
}

/* LOCK CIRCUIT BREAKER.
 * @param:	None.
 * @return:	None.
 */
void ZDJ_Lock(void) {
	// Play sound.
	SOUND_Play(&zdj_lock_sound);
	printf("ZDJ *** Lock.\n");
	fflush(stdout);
}
